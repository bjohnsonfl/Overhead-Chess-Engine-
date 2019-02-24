//
//  evaluate.cpp
//  Chess Engine
//
//  Created by Blake Johnson on 2/21/19.
//  Copyright © 2019 Blake Johnson. All rights reserved.
//

#include "evaluate.hpp"

value pieceValue[numOfPieceType] = {draw, pawnVal, knightVal, bishopVal, rookVal, queenVal};

square oppSq[numOfSq];


int pSqTble[numOfPieceType][numOfSq] =
{
            /*
             {   a1,  b1,  c1,  d1,  e1,  f1,  g1,  h1 },
             {   a2,  b2,  c2,  d2,  e2,  f2,  g2,  h2 },
             {   a3,  b3,  c3,  d3,  e3,  f3,  g3,  h3 },
             {   a4,  b4,  c4,  d4,  e4,  f4,  g4,  h4 },
             {   a5,  b5,  c5,  d5,  e5,  f5,  g5,  h5 },
             {   a6,  b6,  c6,  d6,  e6,  f6,  g6,  h6 },
             {   a7,  b7,  c7,  d7,  e7,  f7,  g7,  h7 },
             {   a8,  b8,  c8,  d8,  e8,  f8,  g8,  h8 },
             */
          
          {//Pawns
                  0,   0,   0,   0,   0,   0,   0,   0 ,
                  5,   5,  10, -10, -10,  -5,   0,   0 ,
                  5,  -5,  -5,  35,  45,  25,   0,   3 ,
                 -5,  -5,  -5,  50,  50,  45,   0,   0 ,
                  0,   0,   0,  30,  30,  15,   0,   0 ,
                  0,   0,   0,   0,   0,   0,   0,   0 ,
                  0,   0,   0,   0,   0,   0,   0,   0 ,
                  0,   0,   0,   0,   0,   0,   0,   0
    
          },
          {//Knights
                -50, -40, -30, -30, -30, -30, -40, -50 ,
                -40, -30,   0,   0,   0,   0, -40, -40 ,
                -40,  10,  15,  30,  30,  30,  10, -40 ,
                -10,  10,  30,  50,  50,  30,   0, -40 ,
                -10,  25,  30,  50,  50,  30,   0, -40 ,
                 20,  35,  35,  35,  35,  35,   0, -40 ,
                 10,  10,  10,  10,  10,  10, -30, -40 ,
                -20, -20, -20, -20, -20, -20, -40, -50
          },
          {//Bishops
                -10, -50, -50, -50, -50, -50, -10,   0 ,
                -50,  30,   0,  30,  30,  30,  35,   0 ,
                -50,  10,  40,  30,  30,  40,  10, -30 ,
                -50,  10,  40,  50,  50,  40,  10, -40 ,
                -50,  10,  40,  50,  50,  40,  10, -40 ,
                -50,  30,  50,  40,  40,  30,  10, -40 ,
                 30,  30,  40,  20,  40,  20,  20, -40 ,
                -50,   0,   0, -30, -30, -30, -30, -10
          },
          {//Rooks
                  0,   0,  50,  50,  50,   0,   0,   0 ,
                  0,   0,   0,  30,  30,   0,   0,   0 ,
                  0,   0,   0,  20,  20,   0,   0,   0 ,
                  0,   0,   0,  20,  20,   0,   0,   0 ,
                  0,   0,   0,  20,  20,   0,   0,   0 ,
                  0,   0,   0,  20,  20,   0,   0,   0 ,
                 50,  50,  50,  50,  50,  50,  50,  50 ,
                  0,   0,   0,  40,  40,   0,   0,   0
          },
          {//Queens
                  0,   0,   0,   0,   0,   0, -10, -20 ,
                  0,   0,   0,  30,  30,  30, -20, -20 ,
                 10,  10,  10,  40,  40,  40,   0, -20 ,
                 20,  40,  50,  60,  60,  40,   0,   0 ,
                 20,  40,  60,  60,  60,  40,   0,   0 ,
                 20,  40,  60,  50,  50,  40,   0,   0 ,
                 20,  40,  60,  50,  50,   0,   0,   0 ,
                  0,   0,  50,  50,  50,   0,   0,   0
          },
          {//Kings
                 10,  50, -50, -50, -50,  -50,  40,   0 ,
                  5, -30, -30, -30, -30, -30, -30, -30 ,
                -40, -40, -40, -40, -40, -40, -40, -40 ,
                -50, -50, -50, -50, -50, -50, -50, -50 ,
                -50, -50, -50, -50, -50, -50, -50, -50 ,
                -50, -50, -50, -50, -50, -50, -50, -50 ,
                -50, -50, -50, -50, -50, -50, -50, -50 ,
                -50, -50, -50, -50, -50, -50, -50, -50 ,
          }
};


void evaluation::init(){
   
    for(square sq = a1; sq < numOfSq; sq++)
    {
        oppSq[sq] = square(sq ^ (black * 56));
    }
}


value evaluate (position& pos)
    {
       
        value eval = pos.get_material(white) - pos.get_material(black);
       
        
        
        bitboard pieces = 0;
        square sq = a1;
        //white pieces
        for(piece pt = w_pawn; pt < b_pawn; pt ++)
        {
            pieces = pos.get_pieces(pt);
            while(pieces)
            {
                sq = lsb_sq(pieces);
                eval =value( eval + pSqTble[pt][sq]);
                pop_lsb(&pieces);
            };
        }
        //black pieces
     
        for(piece pt = b_pawn; pt < numOfPieces; pt ++)
        {
            pieces = pos.get_pieces(pt);
            while(pieces)
            {
                sq = lsb_sq(pieces);
            
               
                eval =value( eval - pSqTble[piece_To_Piecetype(pt)][oppSq[sq]]);
                pop_lsb(&pieces);
            };
        }
        
        
        
        if (pos.get_sideToPlay() == white)
            return eval;
        else return -eval;
    }
