//
//  types.hpp
//  Chess Engine
//
//  Created by Blake Johnson on 12/12/18.
//  Copyright © 2018 Blake Johnson. All rights reserved.
//

#ifndef types_hpp
#define types_hpp

#include <stdio.h>
#include <cstdint>
#include <string>

typedef uint64_t bitboard;

/*
 bit 0 - 2          :           source file
 bit 3 - 5          :           source rank
 bit 0 - 5          :   ////////SOURCE square //////////
 
 bit 6 - 8          :           destination file
 bit 9 - 11         :           destination rank
 bit 6 - 11         :   ////////DESTINATION square /////////
 
 bit 12 - 13        :   //////// Promotion Piece ////////////
 bit 14 - 15        :   //////// Move Flags     ////////////
 */

enum square {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8,
    empty,
    numOfSq = 64
};

enum file {
    file_a, file_b, file_c, file_d, file_e, file_f, file_g, file_h, numOfFile =8, noFile
};

enum rank {
    rank_1, rank_2, rank_3, rank_4, rank_5, rank_6,rank_7, rank_8, numOfRank = 8, noRank
};

inline square operator++(square& d, int) { return d = square(int(d) + 1); }
inline rank operator++(rank& d, int) { return d = rank(int(d) + 1); }
inline rank operator--(rank& d, int) { return d = rank(int(d) - 1); }
inline file operator++(file& d, int) { return d = file(int(d) + 1); }

//compass rose
enum direction {
 noWe = 7, nort = 8, noEa = 9,

 west = -1,          east = 1,
    
 soWe = -9,south = -8, soEa = -7,
    
    
    noNoWe = 15,     noNoEa = 17,

noWeWe = 6,              noEaEa = 10,
    
soWeWe = -10,            soEaEa = -6,
   
    soSoWe = -17,    soSoEa = -15,
    
  
    
center = 0

};

//move use the 12 bits described above the enum square definition
enum move{
    none = 0
};

enum moveFlag {
    regular = 0,
    enPass = 1,
    castling = 2,
    promoting = 3,
};

enum castlingRights {
    noRights,
    W_OO,
    W_OOO = W_OO << 1,
    B_OO = W_OO << 2,
    B_OOO = W_OO << 3
};


enum player {
    white, black, neither, numOfPlayers = 2
};



enum pieceType {
    p_pawn, p_knight, p_bishop, p_rook, p_queen, p_king, numOfPieceType = 6
};

//position.cpp place_peace() is dependent on white being less than 6
enum piece {
    w_pawn, w_knight, w_bishop, w_rook, w_queen, w_king,
    b_pawn, b_knight, b_bishop, b_rook, b_queen, b_king,
    not_piece, numOfPieces = 12
};

enum value {
    draw = 0,
    pawnVal = 100,
    knightVal = 300,
    bishopVal = 330,
    rookVal = 500,
    queenVal = 900,
    mate = 50000    
};

inline piece operator++(piece& d, int) { return d = piece(int(d) + 1); }


const std::string pieceChar = "PNBRQKpnbrqk-";

constexpr square rankFile_to_sq(rank rk, file f)
{
  return  square((rk << 3) + f);
}
constexpr file square_to_file(square sq)
{
    return file(sq & 0x07);
}
constexpr rank square_to_rank(square sq)
{
    return rank((sq >> 3) & 0x07);
}
constexpr square source_sq (move m)
{
    return square(m & 0x3F); //0b 0011 1111 get bottom 6 bits of move
}

constexpr square destination_sq (move m)
{
    return square((m >> 6) & 0x3F); // get the top 6 bits of move
}

constexpr pieceType promo_pieceType (move m)
{
    return pieceType (((m >> 12) & 0x3) + 1);
}

constexpr moveFlag move_flag (move m)
{
    return moveFlag ((m >> 14) & 0x3);
}

constexpr move make_move(square from, square to){
    return move( from | (to << 6));
}
constexpr move make_move(square from, square to, moveFlag flag){
    return move( from | (to << 6) | (flag << 14) );
}

constexpr move make_move(square from, square to, pieceType promo, moveFlag flag){
    return move( from | (to << 6) | (promo << 12) |(flag << 14));
}

#endif /* types_hpp */
