//
//  moveGen.hpp
//  Chess Engine
//
//  Created by Blake Johnson on 2/4/19.
//  Copyright © 2019 Blake Johnson. All rights reserved.
//

#ifndef moveGen_hpp
#define moveGen_hpp

#include <stdio.h>

#include "types.hpp"
#include "bitboard.hpp"
#include "position.hpp"

class position;

enum moveType {
    quiet,
    capture,
    evasion
};

struct candidate {
    move mv;
    int  value;
};

struct candMoveList {
    candidate* end = moveList;
    candidate* start = moveList;
    candidate moveList[256];
};



template <moveType>
candidate* generate (const position& pos, candidate* moveList);

candidate* generateLegal (const position& pos, candidate* moveList, moveType T);
candidate* generateAllLegal (const position& pos, candidate* moveList);

candidate* generateQuiet (const position& pos, candidate* moveList);
candidate* generateCapture (const position& pos, candidate* moveList);
candidate* generatePromotions (const position& pos, candidate* moveList);
candidate* generateEvasion (const position& pos, candidate* moveList);
candidate* generateCastling (const position& pos, candidate* moveList);  //Generates Legal Moves
candidate* generateEnPassent (const position& pos, candidate* moveList);

candidate* makePromotionMoves (square from, square to, candidate* moveList);


//bitboard target can dictate which whether a move will be a capture or not
candidate* knightMoves (bitboard knights, bitboard target, candidate* moveList);
candidate* pawnPushMoves (const position& pos, bitboard pawns, player color, bitboard target, candidate* moveList);
candidate* pawnAttackMoves (bitboard pawns, player color, bitboard target, candidate* moveList);

//for king moves,  get the masks for the kings and xor them so that they cant be a square apart
candidate* kingMoves (const position& pos, bitboard target, candidate* moveList);


//bitboard sliderAttacks (square sq, bitboard occupency, pieceType pt)
candidate* sliderMoves (const position& pos, bitboard target ,candidate* moveList);

void printmove (move m);
void printMoves (candidate* start, candidate* end);












#endif /* moveGen_hpp */
