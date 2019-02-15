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
bool methodA () {
    return true;
}
bool methodB () {
    std::cout << "here\n";
    return true;
}

bool methodC (){
    std::cout <<"methodC\n";
    return false;
}
int main(int argc, const char * argv[]) {
    
    if((methodA() ||  methodB()) && methodC())
    {
        std::cout<<"Pass \n";
        
    }
    else {
        std::cout << "fail \n";
    }
    
    bitboards::init();
    std::string fen= "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    position * pos = new position();
    
    square s = b2;
    square result = numOfSq;
    bitboard c = 1;
    
    result = square(s ^ (c * 56));

    
  
    candMoveList knightMoves;
    knightMoves.end = generateCastling(*pos, knightMoves.moveList);
    printMoves(knightMoves.start, knightMoves.end);
  
 
    
    return 0;
}
