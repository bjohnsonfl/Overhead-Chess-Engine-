//
//  position.cpp
//  Chess Engine
//
//  Created by Blake Johnson on 12/12/18.
//  Copyright © 2018 Blake Johnson. All rights reserved.
//

//TODO:

/*

*/
#include "position.hpp"

position::position()
{
    state = new returnState ();
    reset();
}

void position::reset(){
   
    //these could be overwritten by the fen called in setboard
    ply = 0;
    sideToPlay = white;
    nodes = 0;
    
    //all pieces and square realted things should be handdled in setboard()
    setboard();
    update_check_boards(state);
}

//could overload this function with a fen input so any initial position could be created
void position::setboard(){
    for(square sq =a1; sq < numOfSq; sq++)
    {
        board[sq] = not_piece;
    }
    
    for(piece p = w_pawn; p<numOfPieces ; p++ )
    {
        pieces[p] = 0ULL;
    }
    allWhitePieces = 0ULL;
    allBlackPieces = 0ULL;
    allPieces = allWhitePieces | allBlackPieces;
    
   
    
    
   // std::string fen= "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  //  std::string fen = "2p5/2P5/3pq3/1bR2p1p/4N1B1/2b5/2P2p2/8 W - -";
   // std::string fen ="2p5/2P5/3pq3/1bR2p2/4N1B1/2bQ3p/2P2p2/3p4 w - -";
  //std::string fen = "8/8/5P2/1PPpk3/2KP1N2/1bb5/8/8 w - -";
   // std::string fen =  "7p/6P1/P4pp1/1p3PP1/2Pp2p1/p2P4/1P2p3/p4P2 w - -";
    // std::string fen = "8/pppppPpp/1P6/2P4p/4pP2/1pP5/PPPPPPPp/8 w KQkq -";
    //std::string fen = "r2RQn2/PP1pPPPp/8/1P6/1p6/7p/p1Pp1pp1/bBq1NR2 w - -";
    //std::string fen = "8/8/1b2BB2/1Npk4/2K4r/1nP5/1B1R1Q2/6q1 w - -";
   // std::string fen = "8/8/8/4bb2/8/8/8/R3K1nR w - -";
   // std::string fen ="3Q1R2/3rpP1q/b1Qp1kP1/1P4r1/3r1n1B/rP1KP1r1/1r1P4/B2q1Q2 w - -";
  //  std::string fen ="r1bqkb1r/pp2p1pp/n7/3pPp2/2pP2nN/PP6/2P1BPPP/RNBQK2R b KQkq d3 0 43";
   // std::string fen = "4R2B/k7/8/3PpP2/3K1P2/5NB1/8/4Q3 w - e6 0 2";
    std::string fen = "1q2K3/1QP5/2N5/8/8/8/8/8 w - -v";
    fenParser(fen);
    
    
    //debug
    printAllBitboards();
    printBoard();
    
}

void position::fenParser(std::string &fen){
    
    std::istringstream stream (fen);
    std::string str = "";
    
    rank rk = noRank;
    file f = noFile;
    square sq = empty;
    piece pt = not_piece;
    for(int i =7; i>=0; i--){
        if(i != 0) std::getline(stream, str, '/');
        else stream >> str;
        rk = rank(i);
         f = file_a;
        for (auto it = std::begin(str); it!=std::end(str); ++it)
            {

                switch (*it) {
                        
                    //black pieces
                    case 'p':
                        pt = b_pawn;
                        break;
                    case 'n':
                        pt = b_knight;
                        break;
                    case 'b':
                        pt = b_bishop;
                        break;
                    case 'r':
                        pt = b_rook;
                        break;
                    case 'q':
                        pt = b_queen;
                        break;
                    case 'k':
                        pt = b_king;
                        break;
                        
                    //white pieces
                    case 'P':
                        pt = w_pawn;
                        break;
                    case 'N':
                        pt = w_knight;
                        break;
                    case 'B':
                        pt = w_bishop;
                        break;
                    case 'R':
                        pt = w_rook;
                        break;
                    case 'Q':
                        pt = w_queen;
                        break;
                    case 'K':
                        pt = w_king;
                        break;
                        
                    //file numbers
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                       // std::cout << *it <<std:: endl;
                        f = file(f + file(*it - '1'));
                        pt = not_piece;
                    default:
                        break;
                }
               // std::cout << *it <<std:: endl;
                sq = rankFile_to_sq(rk, f);
                f++;
                place_piece(sq, pt);
            }
        }
    //SIDE TO PLAY
    stream >> str;
    sideToPlay = (str  == "w" || str == "W") ? white : black;
    
    
    //CASTLING RIGHTS
    stream >> str;
    for (auto it = std::begin(str); it!=std::end(str); ++it)
    {
        switch (*it) {
            case '-':
                state -> castling = noRights;
                break;
            case 'K':
                state -> castling |= W_OO;
                break;
            case 'Q':
                 state -> castling |= W_OOO;
                break;
            case 'k':
                 state -> castling |= B_OO;
                break;
            case 'q':
                 state -> castling |= B_OOO;
                break;
                
            default:
                break;
        }
    }
    //ENPASSENT
    stream >> str;
    if( str[0] == '-' ) state -> enPassent = empty;
    else {
        f = file(str[0] - 'a');
        rk = rank ( str[1] - '1');
        sq = rankFile_to_sq(rk, f);
        state -> enPassent = sq;
    }
    
    //Half Move (50 move rule)
    stream >> str;
    int count = 0;
    for (auto it = std::begin(str); it!=std::end(str); ++it)
    {
        if(count == 0) count = *it - '0';
        else {
            count = count * 10;
            count += *it - '0';
        }
    }
    state -> fiftyMove = count;
    
    //Full Move Counter
    stream >> str;
    count = 0;
    for (auto it = std::begin(str); it!=std::end(str); ++it)
    {
        if(count == 0) count = *it - '0';
        else {
            count = count * 10;
            count += *it - '0';
        }
    }
    ply = count;
   
    
}

void position::place_piece(square sq, piece pt)
{
    //do not erase data by placing empty pieces. that is reserved for remove_piece
    player color = neither;
    if(pt != not_piece){
        board[sq] = pt;
        pieces[pt] = pieces[pt] | squareBitMask[sq];
        pt < 6 ? color = white : color = black;
        color == white ? allWhitePieces |= squareBitMask[sq] : allBlackPieces |= squareBitMask[sq];
        allPieces = allWhitePieces | allBlackPieces;
    }
}

void position::remove_piece(square sq, piece pt)
{
        
    player color = neither;
    if(pt != not_piece){
        board[sq] = not_piece;
        pieces[pt] = pieces[pt] ^ squareBitMask[sq];
        pt < 6 ? color = white : color = black;
        color == white ? allWhitePieces ^= squareBitMask[sq] : allBlackPieces ^= squareBitMask[sq];
        allPieces = allWhitePieces | allBlackPieces;
    }
}

void position::update_check_boards(returnState *state)
{
   
    //these are our own pieces pinned due to blocking the attacks on the king
    //The function also passes in pinners which is the their pieces causing the pin
    state -> absolutePinned[white] = pinningBoards(get_king_sq(white), state->pinners[black], white);
   
    state -> absolutePinned[black] = pinningBoards(get_king_sq(black), state->pinners[white], black);

    
}


//Have to bitwise and with opposing pieces to find enemy attackers of the square
bitboard position::squareAttackedBy (square sq) const
{
    
    bitboard attacks =    (attacksFromPawn(sq, black)      & get_pieces(white, p_pawn))
                        | (attacksFromPawn(sq, white)    & get_pieces(black, p_pawn))
                        | (attacksFrom (sq, p_knight)         & get_pieces(p_knight))
                        | (attacksFrom (sq, p_king)           & get_pieces(p_king))
                        | (attacksFrom (sq, p_rook) & get_pieces(  p_rook, p_queen))
                        | (attacksFrom (sq, p_bishop) & get_pieces(p_bishop, p_queen));
    return attacks;
}
//xray functions credit to chessprogramming.org
//These functions return snipers, pieces that attack a square through a piece. this causes pins
bitboard position::xRayRook (bitboard occupied, bitboard ownPieces, square sq) const
{
  
    bitboard attacks = sliderAttacks(sq, occupied, p_rook);
    ownPieces &= attacks;
    return attacks ^ sliderAttacks(sq, occupied ^ ownPieces, p_rook);
}

bitboard position::xRayBishop (bitboard occupied, bitboard ownPieces, square sq) const
{
    
    bitboard attacks = sliderAttacks(sq, occupied, p_bishop);
    ownPieces &= attacks;
    return attacks ^ sliderAttacks(sq, occupied ^ ownPieces, p_bishop);
}

bitboard position:: pinningBoards(square sq, bitboard &pinners, player color)
{

    player us = color;
    player them = player(!us);
    bitboard enemyRookQueen = get_pieces(p_queen, p_rook) & get_pieces(them);
    bitboard enemyBishopQueen = get_pieces(p_queen, p_bishop) & get_pieces(them);
    bitboard snipers = 0, pinned = 0, pinnedHold = 0;
    bitboard ourPieces = get_pieces(us);
    square piece = empty;
    
    snipers = xRayRook(get_pieces(), get_pieces(us), sq) & enemyRookQueen;
    while(snipers)
    {
        piece = square (lsb_sq(snipers));
        pinnedHold |= inBetweenSqBitMask[sq][piece] & ourPieces;
        if(pinnedHold != pinned) pinners |= squareBitMask[piece];
        pinned |= pinnedHold;
        pop_lsb(&snipers);
    }
    
    snipers = xRayBishop(get_pieces(), get_pieces(us), sq) & enemyBishopQueen;
    while(snipers)
    {
        piece = square (lsb_sq(snipers));
        pinnedHold |= inBetweenSqBitMask[sq][piece] & ourPieces;
        if(pinnedHold != pinned) pinners |= squareBitMask[piece];
        pinned |= pinnedHold;
        pop_lsb(&snipers);
    }
    
    return pinned;
}

void position::printAllBitboards(){
    
   
    for(piece p = w_pawn; p<numOfPieces; p++)
    {
        std::cout <<  "Piece: " << pieceChar[p] << "\n";
        printBitboard(pieces[p]);
        std::cout <<  "\n\n\n";
    }
    
    std::cout <<  "all White Pieces: \n";
    printBitboard(allWhitePieces);
    std::cout <<  "\n\n\n";
    
    std::cout <<  "all Black Pieces: \n";
    printBitboard(allBlackPieces);
    std::cout <<  "\n\n\n";
    
    std::cout <<  "ALL Pieces: \n";
    printBitboard(allPieces);
    std::cout <<  "\n\n\n";
}

void position::printBoard(){
    std::cout <<"Current Board: \n";
    std::cout << "---------------\n";
    for(rank r = rank_8; r>=0; r--)
    {
        for(file f = file_a; f<numOfFile; f++)
        {
            std::cout<< pieceChar [ board[rankFile_to_sq(r, f)] ] << " ";
        }
        std::cout <<"\n";
    }
    std::cout << "---------------\n";
}
