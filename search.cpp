//
//  search.cpp
//  Chess Engine
//
//  Created by Blake Johnson on 2/16/19.
//  Copyright © 2019 Blake Johnson. All rights reserved.
//

#include "search.hpp"

move pvTable[256];

bitboard perft (int depth, position& pos)
{
    
    candMoveList moves ;
   
    
    bitboard nodes=0;
    returnState state = returnState();
    if(depth == 0) return 1;
  //  std::cout<<"Depth: "<<depth << "\n";
   // std::cout<<"====================\n";
    if(depth == 1 ){
     // std::cout<<"Depth: "<<depth << "\n";
       // pos.printBoard();
    }
   
  /// moves.end = generateCapture(pos, moves.end);
    moves.end = generateAllLegal(pos, moves.end);
    while(moves.start != moves.end)
    {
        if(pos.legal(moves.start -> mv)){
            
            
           // pos.printBoard();
            //printmove(moves.start -> mv);
            //std::cout<<pos.get_enPassent()  <<"\n";
            if(moves.start -> mv == move(919))// && state.prev->m == move(324))
            {
               // std::cout<<"this move\n";
              //  printBitboard(pos.get_pinned(pos.get_sideToPlay()));
               // pos.printBoard();
               // pos.printAllBitboards();
                
            }
            if(state.captured == b_king || state.captured == w_king)
            {
                //std::cout<<"this move\n";
                //printmove(state.m);
                //printmove(state.prev->m);
                //printmove(moves.start -> mv);
                //pos.printBoard();
            }
            bitboard newBoards [12] ;
            for(int i=0; i<12; i++) newBoards[i] = pos.get_pieces(piece(i));
           
            int whiteCount = popcount(pos.get_pieces(white));
            int blackCount =  popcount(pos.get_pieces(black));
            
            
            //pos.printBoard();
            pos.do_move(moves.start -> mv, state);
            if(pos.get_pieces(pos.get_sideToPlay()) & pos.squareAttackedBy(pos.get_king_sq(player(!pos.get_sideToPlay()))))
            {
                printmove(moves.start -> mv);
                pos.printBoard();
            }
            
            
            if((popcount(pos.get_pieces(white)) < whiteCount) && pos.get_sideToPlay() != white)
            {
                pos.printBoard();
                printmove(moves.start -> mv);
            }
            
            if((popcount(pos.get_pieces(black)) < blackCount) && pos.get_sideToPlay() != black)
            {
                pos.printBoard();
                printmove(moves.start -> mv);
            }
           // printmove(moves.start -> mv);
           // pos.printBoard();
            if( popcount(pos.get_pieces(p_king)) < 2)
            {
                
             //   pos.printAllBitboards();
            }
            nodes += perft(depth -1, pos);
            pos.undo_move(moves.start -> mv);
            
            if(popcount(pos.get_pieces(white)) != whiteCount || popcount(pos.get_pieces(black)) != blackCount)
            {
                pos.printBoard();
            }
            
            
          //  pos.printBoard();
            for(int i=0; i<12; i++) {
              if(  newBoards[i] != pos.get_pieces(piece(i)))
              {
                  pos.printBoard();
              }
            }
            
           // pos.printBoard();
            if( popcount(pos.get_pieces(p_king)) < 2)
            {
               
              //  pos.printAllBitboards();
            }
            if(pos.numberOfPieces() > 32)  {
                std::cout<<"Number oF Pieces: "<< pos.numberOfPieces() << " Depth: " << depth <<"\n";
                std::cout<<"Number oF white: "<< popcount(pos.get_pieces(white)) << "\n";
                std::cout<<"Number oF black: "<< popcount(pos.get_pieces(black)) << "\n";
                std::cout<<"Number oF black Pawns: "<< popcount(pos.get_pieces(black, p_pawn)) << "\n";
                
                std::cout<<pos.get_sideToPlay() << "\n";
                printmove(moves.start -> mv);
                pos.printBoard();
                pos.printAllBitboards();
                printBitboard(pos.get_pieces());
                
            }
           // pos.printBoard();
           // std::cout<< "LEGALLLLLLL\n";
        }
        
        moves.start++;
    }
    return nodes;
    
}


bitboard perftDivide (int depth, position& pos)
{
    
    candMoveList movs;
    
    movs.end = generateAllLegal(pos, movs.end);
    returnState state = returnState();
    bitboard nodes = 0;
    bitboard totalNodes =0;
    while( movs.start != movs.end)
    {
        if(pos.legal(movs.start -> mv))
        {
            pos.do_move(movs.start -> mv, state);
            nodes = perft(depth -1, pos);
            totalNodes += nodes;
            printmove(movs.start->mv);
            std::cout << "nodes: " << nodes << "\n\n";
            pos.undo_move(movs.start -> mv);
        }
        
        movs.start ++;
        
    }
    return totalNodes;
}



move rootSearch (position& pos)
{
    
    alphaBeta(value(-mate), mate, pos, 8);
    pos.bestMove = pvTable[8];
    
    
    return none;
}

value alphaBeta(int alpha, int beta, position& pos, int depth)
{
    
    if( depth == 0) {
        value var = evaluate(pos);

        return var;
    }
    pos.inc_nodes();
    
    bool check = false;
    player us = pos.get_sideToPlay();
    player them = player(!us);
    candMoveList movs;
    returnState state = returnState();
    int legalMoves = 0;
    
    value score = draw;
    value bestScore = value(-mate);
    move bestMove = none;
    
    //Determine if position is in check and if evasions are required. extend depth by 1
    if( pos.get_pieces(them) & pos.squareAttackedBy(pos.get_king_sq(us)))
    {
        check = true;
        //depth++;
    }
    
    movs.end = check == true ? generateEvasion(pos, movs.end)
                             : generateNonEvasions(pos, movs.end);
    candidate * begin = movs.start;
  
    while(begin != movs.end)
    {
        //could use a move picker function here and check for legality in it
        //Only use legal moves
        if(pos.legal(begin -> mv))
        {
            legalMoves++;
            pos.do_move(begin -> mv, state);
            score = value(-alphaBeta(-beta, -alpha, pos, depth - 1));
          /*  if(depth == 8)
            {
                std::cout << "depth: " << depth << " score: " <<score << " best score: " << bestScore <<"\n";
               // printmove(begin -> mv);
                
            }*/
            pos.undo_move(begin -> mv);
            
            //std::cout<< "score: " <<score << " best score: " << bestScore << " depth: " << depth << " alpha: "<< alpha <<"\n";
            
            if(score > bestScore)
            {
               //Current best score at this root, but maybe less than alpha
                bestScore = score;
                bestMove = begin -> mv;
                if(score > value(alpha))
                {
                  
                    /*if(depth == 8)
                   {
                      std::cout<< "score: " <<score << " best score: " << bestScore << " depth: " << depth << " alpha: "<< alpha <<"\n";
                       printmove(begin -> mv);
                   }*/
                    pvTable[depth] = bestMove;
                    pos.bestMove = bestMove;
                   
                    if(score >= value(beta))
                    {
                        return value(beta);
                    }
                    
                    alpha = score;
                    
                }
            }
            
            
        } //legal moves
        
        begin++;
    }//while all moves
    
    
    //mate and statelate
    if((legalMoves == 0) && check)
    {
        return check == true ? value(-mate + pos.get_ply()) : draw;
    }
    
   
    return value(alpha);
}









