//
//  search.hpp
//  Chess Engine
//
//  Created by Blake Johnson on 2/16/19.
//  Copyright © 2019 Blake Johnson. All rights reserved.
//

#ifndef search_hpp
#define search_hpp

#include <stdio.h>

#include "evaluate.hpp"
#include "position.hpp"
#include "moveGen.hpp"
#include "types.hpp"

bitboard perft (int depth,  position& pos);
bitboard perftDivide (int depth , position &pos);

move rootSearch (position& pos);
value alphaBeta (int alpha, int beta, position& pos, int depth);



#endif /* search_hpp */
