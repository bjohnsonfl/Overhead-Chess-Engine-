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
#include "UCI.hpp"

int main(int argc, const char * argv[]) {
    
    
    bitboards::init();
    
    
    UCI newUCI;
    

    
 /*
    position * pos = new position();
    
    bitboard nodes = 0;
    clock_t t;
    std::cout <<"Starting Perft... \n\n";
    t = clock();
    nodes= perftDivide(5, *pos);
    t = clock() - t;
    
    std::cout << "nodes = " << nodes << " seconds: " << float(t)/CLOCKS_PER_SEC << std::endl;
    std::cout << "totalNodes = " << pos->get_nodes() << "\n";
   // pos -> printAllBitboards();
    pos->printBoard();
  //  printBitboard((pos -> get_pieces(p_queen, p_rook)) & pos->get_pieces(white));
    
*/
    return 0;
}
