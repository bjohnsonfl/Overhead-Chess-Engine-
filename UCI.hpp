//
//  UCI.hpp
//  Chess Engine
//
//  Created by Blake Johnson on 2/22/19.
//  Copyright © 2019 Blake Johnson. All rights reserved.
//

#ifndef UCI_hpp
#define UCI_hpp

#include <stdio.h>

#include "position.hpp"

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
    int movetime;
    
    position pos;
    
public:
    UCI ();
    void loop ();
    void parseGo ();
    void position ();
};





#endif /* UCI_hpp */
