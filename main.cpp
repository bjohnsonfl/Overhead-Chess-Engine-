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
    
   

    
  
    candMoveList knightMoves;
    knightMoves.end = generateEvasion(*pos, knightMoves.moveList);
    printMoves(knightMoves.start, knightMoves.end);
  
 
    
    return 0;
}
