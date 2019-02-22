//
//  position.hpp
//  Chess Engine
//
//  Created by Blake Johnson on 12/12/18.
//  Copyright © 2018 Blake Johnson. All rights reserved.
//

#ifndef position_hpp
#define position_hpp

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>

#include "bitboard.hpp"
#include "types.hpp"
struct returnState {
  
    int fiftyMove;
    square enPassent;
    int castling;
    piece captured;
    move m;
    returnState* prev;
    bitboard absolutePinned [numOfPlayers];
    bitboard pinners [numOfPlayers];
    returnState () {
        castling = noRights;  //Always take from the fen
        enPassent = empty;
        fiftyMove = 0;
        captured = not_piece;
        m = none;
        prev = NULL;
        absolutePinned[white] = 0;
        absolutePinned[black] = 0;
        pinners[white] = 0;
        pinners[black] = 0;
    };
    
};
class position {
    
private:
    piece board[numOfSq]; //a board listing every piece at every square for printing purposes
    bitboard pieces[numOfPieces]; //bitboards for each piece, black and white. mulitple pieces of a type will be in them
    bitboard allWhitePieces;
    bitboard allBlackPieces;
    bitboard allPieces;
    
    int ply;
    player sideToPlay;
    bitboard nodes;
    returnState* state;
    
public:
    position();
    void updatePositionFen (std::string fen);
    void reset();
    void setboard();
    void fenParser(std::string& fen);
    
    
    //bitboards and squares
    bitboard get_pieces () const ;
    bitboard get_pieces (player color) const;
    bitboard get_pieces (piece pt) const;
    bitboard get_pieces (pieceType pt) const;
    bitboard get_pieces (pieceType pt, pieceType pt2) const;
    bitboard get_pieces (player color, pieceType pt) const;
    bitboard get_king (player color) const;
    bitboard get_pinned (player color) const;
    square get_king_sq (player color) const;
    square get_enPassent () const;
    
    
    //move related function
    void do_move(move m, returnState& newState);
    void undo_move(move m);
    void place_piece(square sq, piece pt);
    void remove_piece(square sq, piece pt);
    
    //state related functions
    player get_sideToPlay() const;
    void update_check_boards(returnState* state);
    
    //castling
    int get_castlingRights () const;
    int get_castlingRights (player color) const;
    
    //attacks  and checks
    bitboard squareAttackedBy (square sq) const;
    bitboard attacksFrom    (square sq, pieceType pt) const;
    bitboard attacksFromPawn (square sq, player color) const;
    bitboard xRayRook (bitboard occupied, bitboard ownPieces, square sq) const;
    bitboard xRayBishop (bitboard occupied, bitboard ownPieces, square sq) const;
    bitboard pinningBoards (square sq, bitboard& pinners, player color);
    
    //moves
    bool legal(move m) const;
    
    bitboard get_nodes() const;
    void inc_nodes();
    int get_ply() const;
    int numberOfPieces () const;
    void printAllBitboards() const;
    void printBoard() const;
    
    
};


inline bitboard position::get_pieces() const{
    return allPieces;
}

inline bitboard position::get_pieces(player color) const{
    return color == white ? allWhitePieces : allBlackPieces;
}

inline bitboard position::get_pieces(piece pt) const{
    return pieces[pt];
}


inline bitboard position::get_pieces(pieceType pt) const{
    return pieces[pt] | pieces[pt + numOfPieceType] ;
}


inline bitboard position::get_pieces(pieceType pt, pieceType pt2) const{
    return pieces[pt] | pieces[pt + numOfPieceType] | pieces[pt2] | pieces[pt2 + numOfPieceType] ;
}

inline bitboard position::get_pieces(player color, pieceType pt) const{
    return color == white ?   pieces[pt] : pieces[pt + numOfPieceType];
}

inline bitboard position::get_king(player color) const {
    return color == white ?   pieces[w_king] : pieces[b_king];
}

inline bitboard position::get_pinned(player color) const {
    return state -> absolutePinned[color];
}

inline square position::get_king_sq(player color) const
{
    return color == white ?  lsb_sq(pieces[w_king])
    : lsb_sq(pieces[b_king]);
}

inline square position::get_enPassent() const {
    return state->enPassent;
}

inline player position::get_sideToPlay() const{
    return sideToPlay;
}

inline int position::get_castlingRights() const{
    return state-> castling;
}

inline int position::get_castlingRights(player color) const{
    return color == white ? (state-> castling) & 0x3 : (state-> castling) & 0xC ;
}

inline bitboard position::attacksFrom (square sq, pieceType pt) const {
    
    bitboard attacks =  pt == p_pawn ? attacksFromPawn(sq, sideToPlay) :
    pt == p_rook ? sliderAttacks(sq, allPieces, p_rook) :
    pt == p_bishop ? sliderAttacks(sq, allPieces, p_bishop) :
    pt == p_queen ? sliderAttacks(sq, allPieces, p_rook) | sliderAttacks(sq, allPieces, p_bishop) :        openBoardAttacks[pt][sq];
    
    return attacks;
}

inline bitboard position::attacksFromPawn(square sq, player color) const{
    return openBoardPawnAttacks[color][sq];
}

inline bitboard position::get_nodes() const {
    return nodes;
}

inline void position::inc_nodes() {
    nodes++;
}

inline int position::get_ply() const{
    return ply;
}

inline int position::numberOfPieces()const{
    return popcount(allPieces);
}
#endif /* position_hpp */
