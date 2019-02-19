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

#include "position.hpp"
#include "moveGen.hpp"
#include "types.hpp"

bitboard perft (int depth,  position& pos);


#endif /* search_hpp */