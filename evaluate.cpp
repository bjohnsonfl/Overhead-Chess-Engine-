//
//  evaluate.cpp
//  Chess Engine
//
//  Created by Blake Johnson on 2/21/19.
//  Copyright © 2019 Blake Johnson. All rights reserved.
//

#include "evaluate.hpp"



value evaluate (position& pos)
    {
        value var =value ((rand() % 10) );
        
        if (pos.get_sideToPlay() == white)
            return var;
        else return value(-var);
    }
