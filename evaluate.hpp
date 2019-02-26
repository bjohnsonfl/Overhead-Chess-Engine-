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

enum gamePhase {
    middleGame,
    endGame
};

extern square oppSq[numOfSq];
extern int pSqTble[numOfPieceType][numOfSq];
extern int pSqTbleEndGame[numOfPieceType][numOfSq];
extern int gamePhaseCutoff;

value evaluate (position& pos);

#endif /* evaluate_hpp */
