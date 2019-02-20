//
//  main.cpp
//  Chess Engine
//
//  Created by Blake Johnson on 12/12/18.
//  Copyright © 2018 Blake Johnson. All rights reserved.
//

#include <ctime>
#include <iostream>
#include <string>

#include "bitboard.hpp"
#include "moveGen.hpp"
#include "position.hpp"
#include "search.hpp"
#include "types.hpp"


int main(int argc, const char * argv[]) {
    

    
    
    bitboards::init();
 
    position * pos = new position();
    
    bitboard nodes = 0;
    clock_t t;
    std::cout <<"Starting Perft... \n\n";
    t = clock();
    nodes= perft(4, *pos);
    t = clock() - t;
    
    std::cout << "nodes = " << nodes << " seconds: " << float(t)/CLOCKS_PER_SEC << std::endl;
    std::cout << "totalNodes = " << pos->get_nodes() << "\n";
   // pos -> printAllBitboards();
    pos->printBoard();
  //  printBitboard((pos -> get_pieces(p_queen, p_rook)) & pos->get_pieces(white));
    
    
   candMoveList movs;
 //   generate <quiet> (*pos, movs.end );
 //    generate <capture> (*pos, movs.end );
   //  generate <evasion> (*pos, movs.end );
    
    if(pos->get_pieces(player(!(pos->get_sideToPlay())))&pos->squareAttackedBy(pos->get_king_sq(pos->get_sideToPlay()))) {
        // std::cout <<"inside evasion\n";
        // printBitboard(pos.get_pieces(player(!(pos.get_sideToPlay())))&pos.squareAttackedBy(pos.get_king_sq(pos.get_sideToPlay())));
        printBitboard(pos->get_pieces(player(!(pos->get_sideToPlay())))&pos->squareAttackedBy(pos->get_king_sq(pos->get_sideToPlay())));
        pos->printBoard();
        movs.end= generateEvasion(*pos, movs.end);
    }
    else
    {
        printBitboard(pos->get_pieces(player(!(pos->get_sideToPlay())))&pos->squareAttackedBy(pos->get_king_sq(pos->get_sideToPlay())));
         pos->printBoard();
    }
    
    
    
    printMoves(movs.start, movs.end);
    returnState st;
    while(movs.start != movs.end)
    {
        if(pos->legal(movs.start -> mv))
        {
            printmove(movs.start -> mv);
            std::cout<<"legal\n\n";
            pos->do_move(movs.start -> mv, st);
            pos -> printBoard();
            pos->undo_move(movs.start -> mv);
            pos -> printBoard();
            
            
        }
        else {
            printmove(movs.start -> mv);
            std::cout <<"FAIL\n\n";
        }
        movs.start ++;
    }
    
    
  
    candMoveList knightMoves;
    knightMoves.end = generateEvasion(*pos, knightMoves.end);
    printMoves(knightMoves.start, knightMoves.end);
    knightMoves.end = generatePromotions(*pos, knightMoves.end);
    std::cout<< "===================\n";
    printMoves(knightMoves.start, knightMoves.end);
 
    
    return 0;
}
