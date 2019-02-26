//
//  UCI.hpp
//  Chess Engine
//
//  Created by Blake Johnson on 2/22/19.
//  Copyright © 2019 Blake Johnson. All rights reserved.
//

#ifndef UCI_hpp
#define UCI_hpp

#include <ctime>
#include <stdio.h>
#include <string>

#include "evaluate.hpp"
#include "moveGen.hpp"
#include "position.hpp"
#include "search.hpp"
#include "types.hpp"
extern int valueee;
class UCI
{
    
private:
    // Time controls
    int wTime;
    int bTime;
    int wInc;
    int bInc;
    
    //search controls
    int depth;
    int moveTime;
    
    position pos;
    
public:
    UCI ();
    void loop ();
    void parseGo (std::istringstream& stream);
    void parsePosition (std::istringstream& stream, returnState& newState);
    move moveCheck(std::string str); //compare move to legal moves
    std::string moveToString (move m);
    
};





#endif /* UCI_hpp */
