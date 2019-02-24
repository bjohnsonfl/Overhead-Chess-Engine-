//
//  evaluate.hpp
//  Chess Engine
//
//  Created by Blake Johnson on 2/21/19.
//  Copyright © 2019 Blake Johnson. All rights reserved.
//

#ifndef evaluate_hpp
#define evaluate_hpp

#include <stdio.h>

#include "position.hpp"
#include "types.hpp"

class position;

namespace evaluation {
    void init();
}


extern square oppSq[numOfSq];
extern int pSqTble[numOfPieceType][numOfSq];

value evaluate (position& pos);

#endif /* evaluate_hpp */
