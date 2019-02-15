//
//  bitboard.hpp
//  Chess Engine
//
//  Created by Blake Johnson on 12/12/18.
//  Copyright © 2018 Blake Johnson. All rights reserved.
//

#ifndef bitboard_hpp
#define bitboard_hpp




#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "types.hpp"

uint64_t randomU64 ();
uint64_t randomU64_fewbits ();

int popcount (bitboard board);


//lsb is a1, msb is h8
//to change ranks simply shift 8 bits.
//to change files simply shift 1-8 bits
constexpr bitboard board = 0xFFFFFFFFFFFFFFFFULL;

//files
constexpr bitboard fileA = 0x0101010101010101ULL;
constexpr bitboard fileB = fileA << 1;
constexpr bitboard fileC = fileA << 2;
constexpr bitboard fileD = fileA << 3;
constexpr bitboard fileE = fileA << 4;
constexpr bitboard fileF = fileA << 5;
constexpr bitboard fileG = fileA << 6;
constexpr bitboard fileH = fileA << 7;

//ranks
constexpr bitboard rank1 = 0x00000000000000FFULL;
constexpr bitboard rank2 = rank1 << (8 * 1);
constexpr bitboard rank3 = rank1 << (8 * 2);
constexpr bitboard rank4 = rank1 << (8 * 3);
constexpr bitboard rank5 = rank1 << (8 * 4);
constexpr bitboard rank6 = rank1 << (8 * 5);
constexpr bitboard rank7 = rank1 << (8 * 6);
constexpr bitboard rank8 = rank1 << (8 * 7);

//diagonals
constexpr bitboard diagonal = 0x8040201008040201ULL;
constexpr bitboard antiDiagonal = 0x0102040810204080ULL;


//board related bit masks
extern bitboard squareBitMask [numOfSq];
extern bitboard fileBitMask [numOfFile];
extern bitboard rankBitMask [numOfRank];
extern bitboard diagonalBitMask[numOfSq];
extern bitboard antiDiagonalBitMask[numOfSq];
constexpr bitboard lightSqBitMask = 0x55AA55AA55AA55AAULL;
constexpr bitboard darkSqBitMask = ~lightSqBitMask;
constexpr bitboard edgesBitMask = fileA | fileH | rank1 | rank8;
constexpr bitboard cornersMask = 0x8100000000000081ULL;

//magic bitboards
unsigned magicIndex (bitboard blockers, uint64_t magic, int shift);
bitboard sliderAttacks (square sq, bitboard occupency, pieceType pt);


extern bitboard magicRookMask [numOfSq];
extern bitboard magicRookNumbers [numOfSq];
extern bitboard * magicRookIndex [64];
extern bitboard magicRookAttacks [102400];
void initMagicRook ();
constexpr int rookShifts[numOfSq] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12,
};

extern bitboard magicBishopMask [numOfSq];
extern bitboard magicBishopNumbers [numOfSq];
extern bitboard * magicBishopIndex [64];
extern bitboard magicBishopAttacks [5248];
void initMagicBishop ();
constexpr int bishopShifts[numOfSq] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6,
};

//board related clears
extern bitboard clearFile [numOfFile];
extern bitboard clearRank [numOfRank];
constexpr bitboard clearEdges = ~edgesBitMask;
constexpr bitboard clearCorners = ~cornersMask;

//piece related bit masks
extern bitboard openBoardAttacks[numOfPieceType][numOfSq];
extern bitboard openBoardPawnAttacks[2][numOfSq];
extern bitboard inBetweenSqBitMask [numOfSq][numOfSq];
extern bitboard lineSqBitMask [numOfSq][numOfSq]; //calculated in inBetweenBitMask()
void allPawnAttacks();
void allKingAttacks();
void allKnightAttacks();
void allBishopAttacks();
void allRookAttacks();
void allQueenAttacks();
void inBetweenBitMask();

bitboard diagMasks (int sq, bitboard occupied);
bitboard antidiagMasks (int sq, bitboard occupied);
bitboard vertMasks (int sq, bitboard occupied);
bitboard horizMasks (int sq, bitboard occupied);

void printBitboard(bitboard board);

namespace bitboards {
    void init();
}

#ifdef __GNUC__

    inline square lsb_sq(bitboard board)
    {
        
        return square(__builtin_ctzll(board));
    }


    inline void pop_lsb (bitboard * board)
    {
        *board =  *board & (*board -1);
    }

#endif


#endif /* bitboard_hpp */
