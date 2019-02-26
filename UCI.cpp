//
//  UCI.cpp
//  Chess Engine
//
//  Created by Blake Johnson on 2/22/19.
//  Copyright © 2019 Blake Johnson. All rights reserved.
//

#include "UCI.hpp"


UCI::UCI()
{
     wTime = 0;
     bTime = 0;
     wInc = 0;
     bInc = 0;
    
     depth = 0;
     moveTime = 0;
    
}

void UCI::loop(){
    
    
     std::string line ="";
     std::string token = "";
    returnState newState = returnState();
     
     while (std::getline(std::cin, line))
     {
         std::istringstream stream (line);
         token.clear();
         stream >> std::skipws >> token;
       
     
         if(token == "uci")
            {
                std::cout << "id name: Overhead\n";
                std::cout << "id author: Blake Johnson\n";
                std::cout << "uciok\n";
            }
         else if( token == "isready") std::cout << "readyok\n";
         else if( token == "setoption"); //do options later
         else if( token == "register");  //later
         else if( token == "ucinewgame"); //do prep for new game
         else if( token == "position") parsePosition(stream, newState);
         else if (token == "go") parseGo(stream);
         
         //Try and figure out a solution to poll the stream buffer. getline blocks which is not a solution
         //uci says white space before message is possible so peeking is not a solution
         else if (token == "stop");
         else if(token == "quit") break;
     
         else std::cout <<"not a command\n";
     
     
     }
    
    
}

void UCI::parsePosition(std::istringstream &stream, returnState& newState)
{
    logFile.open("log.txt" , std::ofstream::app);
    move m = none;
    std::string token, fen;
    
    stream >> token;
   
    
    if( token == "startpos")
    {
    
        pos.reset();
        stream >> token;
    }
    else if ( token == "fen")
    {
       //credit to stockfish
        while (stream >> token && token != "moves")
            fen += token + " ";
        std::cout<<fen;
         logFile << fen << "\n";
        pos.updatePositionFen(fen);
        pos.printBoard();
    }
  
    else return;
    

    while (stream >> token && ((m = moveCheck(token)) != none))
    {
       // returnState newState = returnState();
       // newState = returnState();
        logFile << token << " ";
        //Ok this is a memory leak. fix this with an array of 256 structs 
        returnState * nnewState = new returnState();
        pos.do_move(m, *nnewState);
        pos.printBoard();
    }
        pos.printBoard();
    
    logFile << "Ply: " << pos.get_ply() << "\n";
    logFile.close();
}

void UCI::parseGo(std::istringstream &stream)
{
    std::string token = "";
    while(stream >> token){
    
        if(token == "wtime") {
            stream >> token;
            wTime = std::stoi(token);
        }
        else if (token == "btime"){
            stream >> token;
            bTime = std::stoi(token);
        }
        else if (token == "winc"){
            stream >> token;
            wInc = std::stoi(token);
        }
        else if (token == "binc"){
            stream >> token;
            bInc = std::stoi(token);
        }
        else if (token == "depth"){
            stream >> token;
            depth = std::stoi(token);
        }
        else if (token == "movetime"){
            stream >> token;
            moveTime = std::stoi(token);
        }
        else if (token == "perft"){
            stream >> token;
            bitboard nodes = 0;
            clock_t t;
            std::cout <<"Starting Perft... \n\n";
            t = clock();
            nodes= perftDivide(5, pos);
            t = clock() - t;
            logFile.open("log.txt",std::ofstream::app);
            std::cout << "nodes = " << nodes << " seconds: " << float(t)/CLOCKS_PER_SEC << std::endl;
            std::cout << "totalNodes = " << pos.get_nodes() << "\n";
            logFile << "nodes = " << nodes << " seconds: " << float(t)/CLOCKS_PER_SEC << std::endl;
            logFile.close();
            
        }
    }
    
    //search
    pos.reset_nodes();
    clock_t t;
    t= clock();
    rootSearch(pos);
    t = clock() - t;
    std::cout << "bestmove " << moveToString(pos.bestMove) << "\n";
   std::cout << "nodes = " << pos.get_nodes() << " seconds: " << float(t)/CLOCKS_PER_SEC << std::endl;
    int nps = pos.get_nodes() / (float(t)/CLOCKS_PER_SEC);
    int k = 1;
    if( (pos.get_material(white) + pos.get_material(black)) >= gamePhaseCutoff) k = 0;
    std::cout << "info score cp "<<valueee<<" nodes " << pos.get_nodes() << " nps " << nps << " string " << k << "\n";
   ;
    
    int totalstates = totalState(pos.get_returnState());
    std::cout << " totalstates: " <<totalstates << "\n";
    
    
    logFile.open("log.txt" , std::ofstream::app);
    logFile << "info score cp "<<valueee<<" nodes " << pos.get_nodes() << " nps " << nps << " string " << k << " totalstates: " <<totalstates <<"\n";
    logFile << "/////////////////////// \n";
    
    
    logFile.close();
}

move UCI::moveCheck(std::string str)
{
    candMoveList movs;
    movs.end = generateAllLegal(pos, movs.end);
    
    while(movs.start != movs.end)
    {
       // std::cout<< moveToString(movs.start -> mv) << "\n";
       // std::cout<<str << "\n";
        if(str == moveToString(movs.start -> mv))
        {
            return movs.start -> mv;
        }
        movs.start++;
    }
    
    return none;
}

std::string UCI::moveToString (move m)
{
    
    //the only internal moves different to external are castling and promotions
    //internally: castling is king to rook sq
    //externally: castling is king src to king dest
    //for promotions, just add the promo letter i.e. fromToPromo
    std::string mv = "    ";
    std::string pieceletters = " nbrq";
    square from ,to;
    from = source_sq(m);
    to = destination_sq(m);
    mv[0] = square_to_file(from) + 'a';
    mv[1] = square_to_rank(from) + '1';
    mv[2] = square_to_file(to) + 'a';
    mv[3] = square_to_rank(to) + '1';
    
    if(move_flag(m) == promoting)
        mv += pieceletters[promo_pieceType(m)];
    
    if(move_flag(m) == castling){
        if(to == a1){
            mv[2] = 'c';
            mv[3] = '1';
        }
        else if(to == h1){
            mv[2] = 'g';
            mv[3] = '1';
        }
        else if(to == a8){
            mv[2] = 'c';
            mv[3] = '8';
        }
        else if(to == h8){
            mv[2] = 'g';
            mv[3] = '8';
        }
    }
    return mv;
}



int UCI::totalState(returnState *st){
    
    if( st == NULL) return 0;
    
    return totalState(st->prev) + 1;
}
