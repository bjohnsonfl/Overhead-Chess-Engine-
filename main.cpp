//
//  main.cpp
//  Chess Engine
//
//  Created by Blake Johnson on 12/12/18.
//  Copyright © 2018 Blake Johnson. All rights reserved.
//

#include <iostream>
#include <string>

#include "bitboard.hpp"
#include "moveGen.hpp"
#include "position.hpp"
#include "types.hpp"


int main(int argc, const char * argv[]) {
    

    
    
    bitboards::init();
 
    position * pos = new position();
    
    candMoveList movs;
    generate <quiet> (*pos, movs.end );
     generate <capture> (*pos, movs.end );
     generate <evasion> (*pos, movs.end );
    
    movs.end = generateCapture (*pos, movs.end);
    printMoves(movs.start, movs.end);
    while(movs.start != movs.end)
    {
        if(pos->legal(movs.start -> mv))
        {
            printmove(movs.start -> mv);
            std::cout<<"legal\n\n";
            
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
