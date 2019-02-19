//
//  search.cpp
//  Chess Engine
//
//  Created by Blake Johnson on 2/16/19.
//  Copyright © 2019 Blake Johnson. All rights reserved.
//

#include "search.hpp"

bitboard perft (int depth, position& pos)
{
    /*
    typedef unsigned long long u64;
    
    u64 Perft(int depth)
    {
        MOVE move_list[256];
        int n_moves, i;
        u64 nodes = 0;
        
        if (depth == 0) return 1;
        
        n_moves = GenerateLegalMoves(move_list);
        for (i = 0; i < n_moves; i++) {
            MakeMove(move_list[i]);
            nodes += Perft(depth - 1);
            UndoMove(move_list[i]);
        }
        return nodes;
    }
     
     */
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
            pos.do_move(moves.start -> mv, state);
            if(pos.get_pieces(pos.get_sideToPlay()) & pos.squareAttackedBy(pos.get_king_sq(player(!pos.get_sideToPlay()))))
            {
                printmove(moves.start -> mv);
                pos.printBoard();
            }
           // printmove(moves.start -> mv);
           // pos.printBoard();
            if( popcount(pos.get_pieces(p_king)) < 2)
            {
                
             //   pos.printAllBitboards();
            }
            nodes += perft(depth -1, pos);
            pos.undo_move(moves.start -> mv);
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
