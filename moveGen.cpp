//
//  moveGen.cpp
//  Chess Engine
//
//  Created by Blake Johnson on 2/4/19.
//  Copyright © 2019 Blake Johnson. All rights reserved.
//

#include "moveGen.hpp"

candidate* generateQuiet(const position& pos, candidate* moveList){
    player us = pos.get_sideToPlay();
    player them = player(!us);
    bitboard quiets = ~(pos.get_pieces());
    bitboard clear7Rank = us == white ? clearRank[rank_8] : clearRank[rank_1];
    
    moveList = kingMoves(pos, quiets, moveList);
    moveList = knightMoves(pos.get_pieces(us, p_knight), quiets, moveList);
    moveList = sliderMoves(pos, quiets, moveList);
    moveList = pawnPushMoves(pos.get_pieces(us, p_pawn), us, quiets & clear7Rank, moveList);
    
    return moveList;
}


//Captures does not include pawns promotions by captures or enpassent.
//For pawn captures on promotion squares, use call the pawn attack function with target of that rank
candidate* generateCapture(const position& pos, candidate* moveList)
{
    
    player us = pos.get_sideToPlay();
    player them = player(!us);
    bitboard captures = pos.get_pieces(them);
    bitboard pawnCaptures = us == white ? captures & clearRank[rank_8] : captures & clearRank[rank_1];
    
    moveList = kingMoves(pos, captures, moveList);
    moveList = knightMoves(pos.get_pieces(us, p_knight), captures, moveList);
    moveList = pawnAttackMoves(pos.get_pieces(us, p_pawn), us, pawnCaptures , moveList);
    moveList = sliderMoves(pos, captures, moveList);
    
    
    
    return moveList;
}

candidate* generatePromotions(const position& pos, candidate* moveList){
 
    player us = pos.get_sideToPlay();
    player them = player(!us);
    bitboard quiets = ~(pos.get_pieces());
    bitboard captures = pos.get_pieces(them);
    bitboard rank7Sq = us == white ? rankBitMask[rank_7] : rankBitMask[rank_2];
    bitboard rank8Sq = us == white ? rankBitMask[rank_8] : rankBitMask[rank_1];
    
    moveList = pawnPushMoves(pos.get_pieces(us, p_pawn) & rank7Sq, us, quiets & rank8Sq, moveList);
    
    moveList = pawnAttackMoves(pos.get_pieces(us, p_pawn) & rank7Sq, us, captures & rank8Sq, moveList);
    
    return moveList;
}

candidate* generateEvasion (const position& pos, candidate* moveList)
{
    /*If in check, the king has 2-3 options.
     1) Move out of Check
     2) Block Check
     3) Capture Check
     Option 1 is required when more than one piece is checking the king
     
     step 1) determine which pieces are attacking the king.
     step 2) generate attack squares from this piece and AND them with king moveable squares
        step 2.1) Make sure the enemy king squares are not included
        step 2.2) Determine if these squares are attacked, if not, create move
     step 3) calculate how many attackers there are from step 1. if more than 1, Break, else step 4
     step 4) Determine which squares are between the king and the attacker, see if our pieces attack them, are not pinned, and thus can move there to BLOCK the check
        step 4.1) promotions count
     step 5) Determine if our pieces can capture the attacking piece, and are not pinned
        step 5.1) EnPassant counts as well as promotions
    
     */
    
    
    
    
    
    
    
    
    return moveList;
}






//Not compatible with chess 960. Only called if castling rights are true.
//THIS FUNCTION IS ONLY CALLED IF DETERMINED CASTLING RIGHTS EXIST FIRST
//Must check to see if king or squares are in check or under attack
//Must check to see if king and rook path are clear
//Assumes rook position is on the A or H file in standard chess
//The Move will be encoded as from = king_square to = rook_square
//Do_move() must decipher the correct destination squares based on rook source
candidate* generateCastling(const position& pos, candidate* moveList)
{
    player us = pos.get_sideToPlay();
    player them = player(!us);
    bitboard occupied  = pos.get_pieces();
    bitboard enemy = pos.get_pieces(them);
    int castlightRights = us == white ? pos.get_castlingRights(white) : pos.get_castlingRights(black) >> 2;
    square sq, kSq; //rkSq
    bool illegal = false;
    kSq = lsb_sq(pos.get_king(us));
    
    illegal = ((pos.squareAttackedBy(kSq)) & enemy) == true ? true :false;
    
    if((castlightRights & 0x1) && !illegal) // KING SIDE CASTLE
    {
        sq = square(kSq + 1);
        while(!illegal && (sq < h1))
        {
            if(squareBitMask[sq] & occupied) illegal = true;
            if((pos.squareAttackedBy(sq)) & enemy) illegal = true;
           
            sq = square( sq + 1 );
        }
        
        if(!illegal)
        {
            //make move
            (*moveList++).mv = make_move(kSq, h1, pieceType(0) , castling);
            
        }
        
        
        illegal = false;
    }
    if ((castlightRights & 0x2) && !illegal) // QUEEN SIDE CASTLE
    {
        sq = square(kSq - 1);
        while(!illegal && (sq > b1))
        {
            if(squareBitMask[sq] & occupied) illegal = true;
            if((pos.squareAttackedBy(sq)) &enemy) illegal = true;
            printBitboard(pos.squareAttackedBy(sq));
            printBitboard(pos.squareAttackedBy(sq) & enemy);
            
            sq = square( sq - 1 );
        }
        
        if(!illegal)
        {
            //make move
            (*moveList++).mv = make_move(kSq, a1, pieceType(0) , castling);
           
        }
    }
    
    
    return moveList;
}

//ONLY CALL IF ENPASSENT SQUARE EXISTS
candidate* generateEnPassent (const position& pos, candidate* moveList)
{
    player us = pos.get_sideToPlay();
   // player them = player(!us);
   // bitboard occupied  = pos.get_pieces();
//bitboard enemy = pos.get_pieces(them);
    square enPsq = pos.get_enPassent();
    
    square capturedPawn, leftPawn, rightPawn;
    capturedPawn = us == white ? square(enPsq + south) : square(enPsq + nort);
    leftPawn = square(capturedPawn + west);
    rightPawn = square (capturedPawn + east);
    
    bitboard leftPawnBB, rightPawnBB, pawns; // postEP, capturePawnBB,
    leftPawnBB = squareBitMask[leftPawn];
    rightPawnBB = squareBitMask[rightPawn];
    pawns = pos.get_pieces(us, p_pawn);
    
    if(pawns & leftPawnBB)
    {
        (*moveList++).mv = make_move(leftPawn, enPsq, pieceType(0) ,enPass);

    }
    
    
    if(pawns & rightPawnBB)
    {
        
        (*moveList++).mv = make_move(rightPawn, enPsq, pieceType(0) ,enPass);
        
    }
    
    
    return moveList;
}

candidate* makePromotionMoves (square from, square to, candidate* moveList)
{
    (*moveList++).mv = make_move(from, to, pieceType(p_knight - 1), promoting);
    (*moveList++).mv = make_move(from, to, pieceType(p_bishop - 1), promoting);
    (*moveList++).mv = make_move(from, to, pieceType(p_rook - 1), promoting);
    (*moveList++).mv = make_move(from, to, pieceType(p_queen - 1), promoting);
    
    return moveList;
}


candidate* knightMoves (bitboard knights,bitboard target, candidate* moveList)
{
    bitboard squares = 0;
    square from = empty, to = empty;
    move mov = none;
    while(knights)
    {
        from =lsb_sq(knights);
        //check now if the from square is absolute pinned
        squares = target & openBoardAttacks[p_knight][from];
        
        while(squares)
        {
            //could check here if the to square places their king in check
            to  = lsb_sq(squares);
            mov = make_move(from, to);
            (*moveList++).mv = mov;
            
            pop_lsb(&squares);
        }
        pop_lsb(&knights);
    }
    return moveList;
}


//Target MUST be open squares ONLY. Clear 1st/8th rank to avoid promotions per color
candidate* pawnPushMoves (bitboard pawns, player color, bitboard target, candidate* moveList){
    
    direction shift = color == white ? nort : south;
    bitboard rank8Sq = color == white ? rankBitMask[rank_8] : rankBitMask[rank_1];
    
    square from = empty, to = empty;
    move mov = none;
    
    bitboard singlePush = color == white ? (pawns << nort) & target : (pawns >> -south) & target;
    bitboard doublePush = color == white ? (singlePush & rankBitMask[rank_3]) << nort
                                         : (singlePush & rankBitMask[rank_6]) >> -south;
    doublePush &= target;
    printBitboard(pawns);
    printBitboard(singlePush);
    printBitboard(doublePush);
    
    while(singlePush)
    {
        to = lsb_sq(singlePush);
        from = square(to - shift);
        if ( squareBitMask[to] & rank8Sq ) {
           moveList = makePromotionMoves(from, to, moveList);
        }
        else {
            mov = make_move(from, to);
            (*moveList++).mv = mov;
        }
        
        pop_lsb(&singlePush);
    }
    
    //can never promote
    while(doublePush)
    {
        to = lsb_sq(doublePush);
        from = square(to - shift - shift);
        mov = make_move(from, to);
        (*moveList++).mv = mov;
        
        pop_lsb(&doublePush);
    }

    return moveList;
}


candidate* pawnAttackMoves (bitboard pawns, player color ,bitboard target, candidate* moveList){
    
    bitboard squares = 0;
    square from = empty, to = empty;
    move mov = none;
    bitboard rank8Sq = color == white ? rankBitMask[rank_8] : rankBitMask[rank_1];
    
    while(pawns)
    {
        from = lsb_sq(pawns);
        //check now if the from square is absolute pinned
        squares = target & openBoardPawnAttacks[color][from];
        while(squares)
        {
            //could check here if the to square places their king in check
            to  = lsb_sq(squares);
            
            if (squareBitMask[to]  & rank8Sq) {
               moveList = makePromotionMoves(from, to, moveList);
            }
            else{
                mov = make_move(from, to);
                (*moveList++).mv = mov;
            }
            pop_lsb(&squares);
        }
        pop_lsb(&pawns);
        
    }
    
    return moveList;
}

candidate* kingMoves (const position& pos, bitboard target, candidate* moveList){
    
    bitboard squares = 0, enemyKing, king;
    square from = empty, to = empty;
    move mov  = none;
    player color = pos.get_sideToPlay();
    king = pos.get_king(pos.get_sideToPlay());
    
   
    from = lsb_sq(king);
    squares = openBoardAttacks[p_king][from];
    enemyKing = openBoardAttacks[p_king][pos.get_king_sq(player(!color))];
    squares = squares ^( squares & enemyKing );
    squares &= target;
    
    while(squares)
    {
        //could check here if the to square places their king in check
        to  = lsb_sq(squares);
        mov = make_move(from, to);
        (*moveList++).mv = mov;
        
        pop_lsb(&squares);
    }

    return moveList;
}


//bitboard sliderAttacks (square sq, bitboard occupency, pieceType pt)
candidate* sliderMoves (const position& pos, bitboard target, candidate* moveList)
{
    // if target is of empty squares, its quiet moves.
    // if target is of enemy pieces, its captures.
    bitboard occupency, rooks, rookSquares, bishops, bishopSquares, queens, queenSquares;
    square from = empty, to = empty;
    move mov = none;
    player color = pos.get_sideToPlay();
    occupency = pos.get_pieces();
    rooks = pos.get_pieces(color, p_rook);
    bishops = pos.get_pieces(color, p_bishop);
    queens = pos.get_pieces(color, p_queen);
    
    
    while(rooks)
    {
        from = lsb_sq(rooks);
        //check now if the from square is absolute pinned
        rookSquares = target & sliderAttacks(from, occupency, p_rook);
        while(rookSquares)
        {
             //could check here if the to square places their king in check
            to = lsb_sq(rookSquares);
            mov = make_move(from, to);
            (*moveList++).mv = mov;
            pop_lsb(&rookSquares);
        }
        pop_lsb(&rooks);
    }
    
    while(bishops)
    {
        from = lsb_sq(bishops);
        //check now if the from square is absolute pinned
        bishopSquares = target & sliderAttacks(from, occupency, p_bishop);
        
        while(bishopSquares)
        {
            //could check here if the to square places their king in check
            to = lsb_sq(bishopSquares);
            mov = make_move(from, to);
            (*moveList++).mv = mov;
            pop_lsb(&bishopSquares);
        }
        pop_lsb(&bishops);
    }
    
    while(queens)
    {
        from = lsb_sq(queens);
        //check now if the from square is absolute pinned
        queenSquares = target & (sliderAttacks(from, occupency, p_rook) | sliderAttacks(from, occupency, p_bishop));
        printBitboard(queenSquares);
        while(queenSquares)
        {
            //could check here if the to square places their king in check
            to = lsb_sq(queenSquares);
            mov = make_move(from, to);
            (*moveList++).mv = mov;
            pop_lsb(&queenSquares);
        }
        pop_lsb(&queens);
    }
    
    
    
    
    
    return moveList;
}

void printmove (move m)
{
    square from ,to;
    char mv [4];
    
        from = source_sq(m);
    to = destination_sq(m);
        mv[0] = square_to_file(from) + 'A';
        mv[1] = square_to_rank(from) + '1';
        mv[2] = square_to_file(to) + 'A';
        mv[3] = square_to_rank(to) + '1';
        std::cout << mv[0] << mv[1] << mv[2] << mv[3] << "\n";
    
}

void printMoves (candidate* start, candidate* end)
{
    std::string pieceletters = "_NBRQ";
    std::string flagletters = "_recp";
    square from ,to;
    char mv [7];
    
    while( start!= end)
    {
        from = source_sq(start-> mv);
        to = destination_sq(start -> mv);
        mv[0] = square_to_file(from) + 'A';
        mv[1] = square_to_rank(from) + '1';
        mv[2] = square_to_file(to) + 'A';
        mv[3] = square_to_rank(to) + '1';
        mv[4] = ' ';
        mv[5] = pieceletters[promo_pieceType(start->mv)] ;
        mv[6] = flagletters[move_flag(start->mv)+1];
        std::cout << mv[0] << mv[1] << mv[2] << mv[3] << mv[4] << mv[5] << mv[6]  << "\n";
         start++;
    }
}


