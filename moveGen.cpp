//
//  moveGen.cpp
//  Chess Engine
//
//  Created by Blake Johnson on 2/4/19.
//  Copyright © 2019 Blake Johnson. All rights reserved.
//

#include "moveGen.hpp"


template<>
candidate* generate <quiet> (const position& pos, candidate* moveList)
{
   moveList = generateQuiet(pos, moveList);
    
    return moveList;
}

template<> candidate* generate <capture> (const position& pos, candidate* moveList)
{
     moveList = generateCapture(pos, moveList);
    
    return moveList;
}

template<> candidate* generate <evasion> (const position& pos, candidate* moveList)
{
    moveList = generateEvasion(pos, moveList);
    
    return moveList;
}

candidate* generateLegal (const position& pos, candidate* moveList, moveType T)
{
    moveList = T == quiet   ? generate <quiet>   (pos, moveList):
    T == capture ? generate <capture> (pos, moveList):
    generate <evasion> (pos, moveList);
    
    
    return moveList;
}

candidate* generateNonEvasions(const position& pos, candidate* moveList)
{
    moveList = generateCapture(pos, moveList);
    moveList = generatePromotions(pos, moveList);
    moveList = generateQuiet(pos, moveList);
    if(pos.get_castlingRights(pos.get_sideToPlay()))moveList = generateCastling(pos, moveList);
    if(pos.get_enPassent() != empty) moveList = generateEnPassent(pos, moveList);

return moveList;
}


candidate* generateAllLegal (const position& pos, candidate* moveList)
{
   
    if(pos.get_pieces(player(!(pos.get_sideToPlay()))) & pos.squareAttackedBy(pos.get_king_sq(pos.get_sideToPlay()))) {
       //std::cout <<"inside evasion\n";
       // printBitboard(pos.get_pieces(player(!(pos.get_sideToPlay())))&pos.squareAttackedBy(pos.get_king_sq(pos.get_sideToPlay())));
       // pos.printBoard();
        
        return generateEvasion(pos, moveList);
    }
    else{
       //  std::cout <<"inside else\n";
    
    moveList = generateCapture(pos, moveList);
    moveList = generatePromotions(pos, moveList);
    moveList = generateQuiet(pos, moveList);
    if(pos.get_castlingRights(pos.get_sideToPlay()))moveList = generateCastling(pos, moveList);
    if(pos.get_enPassent() != empty) moveList = generateEnPassent(pos, moveList);
    }
   
    
    return moveList;
}


candidate* generateQuiet(const position& pos, candidate* moveList){
    player us = pos.get_sideToPlay();
    player them = player(!us);
    bitboard quiets = ~(pos.get_pieces());
    bitboard clear7Rank = us == white ? clearRank[rank_8] : clearRank[rank_1];
    
    moveList = sliderMoves(pos, quiets, moveList);
    moveList = pawnPushMoves(pos, pos.get_pieces(us, p_pawn), us, quiets & clear7Rank, moveList);
    moveList = kingMoves(pos, quiets, moveList);
    moveList = knightMoves(pos.get_pieces(us, p_knight), quiets, moveList);
    
    
    return moveList;
}


//Captures does not include pawns promotions by captures or enpassent.
//For pawn captures on promotion squares, use call the pawn attack function with target of that rank
candidate* generateCapture(const position& pos, candidate* moveList)
{
    
    player us = pos.get_sideToPlay();
    player them = player(!us);
    bitboard captures = pos.get_pieces(them);
    bitboard pawnCaptures = us == white ? captures & clearRank[rank_8] : captures & clearRank[rank_1];
    
    moveList = kingMoves(pos, captures, moveList);
    moveList = knightMoves(pos.get_pieces(us, p_knight), captures, moveList);
    moveList = pawnAttackMoves(pos.get_pieces(us, p_pawn), us, pawnCaptures , moveList);
    moveList = sliderMoves(pos, captures, moveList);
    
    
    
    return moveList;
}

candidate* generatePromotions(const position& pos, candidate* moveList){
 
    
    player us = pos.get_sideToPlay();
    player them = player(!us);
    bitboard quiets = ~(pos.get_pieces());
    bitboard captures = pos.get_pieces(them);
    bitboard rank7Sq = us == white ? rankBitMask[rank_7] : rankBitMask[rank_2];
    bitboard rank8Sq = us == white ? rankBitMask[rank_8] : rankBitMask[rank_1];
    
    moveList = pawnPushMoves(pos, pos.get_pieces(us, p_pawn) & rank7Sq, us, quiets & rank8Sq, moveList);
    
    moveList = pawnAttackMoves(pos.get_pieces(us, p_pawn) & rank7Sq, us, captures & rank8Sq, moveList);
    
    return moveList;
}

candidate* generateEvasion (const position& pos, candidate* moveList)
{
    /*If in check, the king has 2-3 options.
     1) Move out of Check
     2) Block Check
     3) Capture Check
     Option 1 is required when more than one piece is checking the king
     
     step 1) determine which pieces are attacking the king.
     step 2) generate attack squares from this piece and AND them with king moveable squares
        step 2.1) Make sure the enemy king squares are not included
        step 2.2) Determine if these squares are attacked, if not, create move
     step 3) calculate how many attackers there are from step 1. if more than 1, Break, else step 4
     step 4) Determine which squares are between the king and the attacker, see if our pieces attack them, are not pinned, and thus can move there to BLOCK the check
        step 4.1) promotions count
     step 5) Determine if our pieces can capture the attacking piece, and are not pinned
        step 5.1) EnPassant counts as well as promotions
    
     */
    candidate *start = moveList;
  //  pos.printBoard();
    square sq = empty, to = empty, from = empty;
    move mov = none;
    player us = pos.get_sideToPlay();
    player them = player(!us);
    
    square kingSq = pos.get_king_sq(us);
    square enemyKingSq = pos.get_king_sq(them);
    
    bitboard kingSquares = openBoardAttacks[p_king][kingSq];
    bitboard enemyKingSquares = openBoardAttacks[p_king][enemyKingSq];
    
    //To Keep kings apart. This bitboard is all possible squares a king can move to now
    kingSquares = kingSquares ^( kingSquares & enemyKingSquares );
    //printBitboard(kingSquares);
    //Step 1/2)
    bitboard checker = pos.squareAttackedBy(kingSq) & pos.get_pieces(them);
    bitboard sliders =( pos.get_pieces(p_queen, p_bishop) | pos.get_pieces(p_rook)) & checker;
    bitboard checkedSquares = 0;
   // printBitboard(checker);
    //printBitboard(sliders);
    while(sliders)
    {
        sq = lsb_sq(sliders);
       // printBitboard(lineSqBitMask[sq][kingSq]);
        checkedSquares |= lineSqBitMask[sq][kingSq] ^ squareBitMask[sq];
        pop_lsb(&sliders);
        
    }
   // printBitboard(checkedSquares);
    kingSquares &= ~checkedSquares;
    //printBitboard(kingSquares);
    bitboard kingCaptures = 0, kingQuiets = 0;
    kingCaptures = kingSquares & pos.get_pieces(them);
   // printBitboard(kingCaptures);
    kingQuiets  = kingSquares & ~(pos.get_pieces());
   // printBitboard(kingQuiets);
    while(kingCaptures)
    {
        to = lsb_sq(kingCaptures);
        mov = make_move(kingSq, to);
        (*moveList++).mv = mov;
        
        pop_lsb(&kingCaptures);
    }
    
    while(kingQuiets)
    {
        to = lsb_sq(kingQuiets);
        mov = make_move(kingSq, to);
        (*moveList++).mv = mov;
        
        pop_lsb(&kingQuiets);
    }
    
    //Step 3)
    //these moves will be checked for legality later
    if( popcount(checker) > 1) return moveList;
    
    //Step 4)
    //Pawn (and promotion), knight, bishop, rook, queen
    square checkerSq = lsb_sq(checker);
    bitboard blockerSq = inBetweenSqBitMask[kingSq][checkerSq];
    //printBitboard(blockerSq);
    moveList = pawnPushMoves(pos, pos.get_pieces(us, p_pawn), us, blockerSq, moveList);
    moveList = knightMoves(pos.get_pieces(us, p_knight), blockerSq, moveList);
    moveList = sliderMoves(pos, blockerSq, moveList);
    
    //Step 5)
    bitboard capture = squareBitMask[checkerSq];
    moveList = pawnAttackMoves(pos.get_pieces(us, p_pawn), us, capture, moveList);
    if(pos.get_enPassent() != empty)
    {
        moveList = generateEnPassent(pos, moveList);
    }
    moveList = knightMoves(pos.get_pieces(us, p_knight), capture, moveList);
    moveList = sliderMoves(pos, capture, moveList);
    
  //  printMoves(start, moveList);
    
    return moveList;
}






//Not compatible with chess 960. Only called if castling rights are true.
//THIS FUNCTION IS ONLY CALLED IF DETERMINED CASTLING RIGHTS EXIST FIRST
//Must check to see if king or squares are in check or under attack
//Must check to see if king and rook path are clear
//Assumes rook position is on the A or H file in standard chess
//The Move will be encoded as from = king_square to = rook_square
//Do_move() must decipher the correct destination squares based on rook source
candidate* generateCastling(const position& pos, candidate* moveList)
{
    player us = pos.get_sideToPlay();
    player them = player(!us);
    bitboard occupied  = pos.get_pieces();
    bitboard enemy = pos.get_pieces(them);
    int castlightRights = us == white ? pos.get_castlingRights(white) : pos.get_castlingRights(black) >> 2;
    square sq, kSq; //rkSq
    bool illegal = false;
    kSq = lsb_sq(pos.get_king(us));
    
    illegal = ((pos.squareAttackedBy(kSq)) & enemy) == true ? true :false;
    
    square a_1 = us == white ? a1 : a8;
    square b_1 = us == white ? b1 : b8;
    square h_1 = us == white ? h1 : h8;
    
    if((castlightRights & 0x1) && !illegal) // KING SIDE CASTLE
    {
        sq = square(kSq + 1);
        while(!illegal && (sq < h_1))
        {
            if(squareBitMask[sq] & occupied) illegal = true;
            if((pos.squareAttackedBy(sq)) & enemy) illegal = true;
           
            sq = square( sq + 1 );
        }
        
        if(!illegal)
        {
            //make move
            (*moveList++).mv = make_move(kSq, h_1, pieceType(0) , castling);
            
        }
        
        
        illegal = false;
    }
    if ((castlightRights & 0x2) && !illegal) // QUEEN SIDE CASTLE
    {
        sq = square(kSq - 1);
        //BUGGGGGGGGGG b1 is never cheked
        while(!illegal && (sq > a_1))
        {
            if(squareBitMask[sq] & occupied) illegal = true;
            if(((pos.squareAttackedBy(sq)) & enemy) && sq != b_1) illegal = true;
           // printBitboard(pos.squareAttackedBy(sq));
           // printBitboard(pos.squareAttackedBy(sq) & enemy);
            
            sq = square( sq - 1 );
        }
        
        if(!illegal)
        {
            //make move
            (*moveList++).mv = make_move(kSq, a_1, pieceType(0) , castling);
           
        }
    }
    
    
    return moveList;
}

//ONLY CALL IF ENPASSENT SQUARE EXISTS
candidate* generateEnPassent (const position& pos, candidate* moveList)
{
   
    player us = pos.get_sideToPlay();
    // player them = player(!us);
    // bitboard occupied  = pos.get_pieces();
    //bitboard enemy = pos.get_pieces(them);
    square enPsq = pos.get_enPassent();
    
    square capturedPawn, leftPawn, rightPawn;
    capturedPawn = us == white ? square(enPsq + south) : square(enPsq + nort);
    
    //TO avoid wrapping off the board to the next rank,
    //A file and H file squares must be avoided for left and right accordingly
    //Set them to a1 as it is impossible for a pawn to be in the 1st rank, the if later will be false
    //however if a bug causes a pawn to be on a1 by a faulty promotion, not good.
    //Think of a more robust fix later.
    //just hope promotions never fail!
    
    leftPawn = square_to_file(capturedPawn) != file_a ? square(capturedPawn + west) : a1;
    rightPawn = square_to_file(capturedPawn) != file_h ? square (capturedPawn + east) : a1;
    
    bitboard leftPawnBB, rightPawnBB, pawns; // postEP, capturePawnBB,
    leftPawnBB = squareBitMask[leftPawn];
    rightPawnBB = squareBitMask[rightPawn];
    pawns = pos.get_pieces(us, p_pawn);
    
    if(pawns & leftPawnBB)
    {
        (*moveList++).mv = make_move(leftPawn, enPsq, pieceType(0) ,enPass);

    }
    
    
    if(pawns & rightPawnBB)
    {
        
        (*moveList++).mv = make_move(rightPawn, enPsq, pieceType(0) ,enPass);
        
    }
    
    
    return moveList;
}

candidate* makePromotionMoves (square from, square to, candidate* moveList)
{
    //(*moveList++).mv = make_move(from, to, pieceType(p_knight - 1), promoting);
    //(*moveList++).mv = make_move(from, to, pieceType(p_bishop - 1), promoting);
    //(*moveList++).mv = make_move(from, to, pieceType(p_rook - 1), promoting);
    (*moveList++).mv = make_move(from, to, pieceType(p_queen - 1), promoting);
    
    return moveList;
}


candidate* knightMoves (bitboard knights,bitboard target, candidate* moveList)
{
    bitboard squares = 0;
    square from = empty, to = empty;
    move mov = none;
    while(knights)
    {
        from =lsb_sq(knights);
        //check now if the from square is absolute pinned
        squares = target & openBoardAttacks[p_knight][from];
        
        while(squares)
        {
            //could check here if the to square places their king in check
            to  = lsb_sq(squares);
            mov = make_move(from, to);
            (*moveList++).mv = mov;
            
            pop_lsb(&squares);
        }
        pop_lsb(&knights);
    }
    return moveList;
}


//Target MUST be open squares ONLY. Clear 1st/8th rank to avoid promotions per color
candidate* pawnPushMoves (const position& pos, bitboard pawns, player color, bitboard target, candidate* moveList){
    
    direction shift = color == white ? nort : south;
    bitboard rank8Sq = color == white ? rankBitMask[rank_8] : rankBitMask[rank_1];
    
    
    square from = empty, to = empty;
    move mov = none;
    
    bitboard unoccupied = ~(pos.get_pieces());
    
    //BUG!!! Here if a single push target doesnt exist, then a double push is impossible
    //BUT if a double push target exists, it will never occur because the single never happened
   // bitboard singlePush = color == white ? (pawns << nort) & target : (pawns >> -south) & target;
    //bitboard doublePush = color == white ? (singlePush & rankBitMask[rank_3]) << nort
   //                                      : (singlePush & rankBitMask[rank_6]) >> -south;
    
    
    //PROBLEM AGAIN. this allows for double pawn push over an occupied square. fix it
     bitboard singlePush = color == white ? (pawns << nort)  : (pawns >> -south);
    singlePush &= unoccupied;
     bitboard doublePush = color == white ? (singlePush & rankBitMask[rank_3]) << nort
                                        : (singlePush & rankBitMask[rank_6]) >> -south;
    singlePush &= target;
    doublePush &= target;
   // printBitboard(pawns);
   // printBitboard(singlePush);
  //  printBitboard(doublePush);
    
    while(singlePush)
    {
        to = lsb_sq(singlePush);
        from = square(to - shift);
        if ( squareBitMask[to] & rank8Sq ) {
           moveList = makePromotionMoves(from, to, moveList);
        }
        else {
            mov = make_move(from, to);
            (*moveList++).mv = mov;
        }
        
        pop_lsb(&singlePush);
    }
    
    //can never promote
    while(doublePush)
    {
        to = lsb_sq(doublePush);
        from = square(to - shift - shift);
        mov = make_move(from, to);
        (*moveList++).mv = mov;
        
        pop_lsb(&doublePush);
    }

    return moveList;
}


candidate* pawnAttackMoves (bitboard pawns, player color ,bitboard target, candidate* moveList){
    
    bitboard squares = 0;
    square from = empty, to = empty;
    move mov = none;
    bitboard rank8Sq = color == white ? rankBitMask[rank_8] : rankBitMask[rank_1];
    
    while(pawns)
    {
        from = lsb_sq(pawns);
        //check now if the from square is absolute pinned
        squares = target & openBoardPawnAttacks[color][from];
        while(squares)
        {
            //could check here if the to square places their king in check
            to  = lsb_sq(squares);
            
            if (squareBitMask[to]  & rank8Sq) {
               moveList = makePromotionMoves(from, to, moveList);
            }
            else{
                mov = make_move(from, to);
                (*moveList++).mv = mov;
            }
            pop_lsb(&squares);
        }
        pop_lsb(&pawns);
        
    }
    
    return moveList;
}

candidate* kingMoves (const position& pos, bitboard target, candidate* moveList){
    
    bitboard squares = 0, enemyKing, king;
    square from = empty, to = empty;
    move mov  = none;
    player color = pos.get_sideToPlay();
    king = pos.get_king(color);
    
   
    from = lsb_sq(king);
    squares = openBoardAttacks[p_king][from];
    enemyKing = openBoardAttacks[p_king][pos.get_king_sq(player(!color))];
    squares = squares ^( squares & enemyKing );
    squares &= target;
    
    
    while(squares)
    {
        //could check here if the to square places their king in check
        to  = lsb_sq(squares);
        mov = make_move(from, to);
        (*moveList++).mv = mov;
        
        pop_lsb(&squares);
    }

    return moveList;
}


//bitboard sliderAttacks (square sq, bitboard occupency, pieceType pt)
candidate* sliderMoves (const position& pos, bitboard target, candidate* moveList)
{
    // if target is of empty squares, its quiet moves.
    // if target is of enemy pieces, its captures.
    bitboard occupency, rooks, rookSquares, bishops, bishopSquares, queens, queenSquares;
    square from = empty, to = empty;
    move mov = none;
    player color = pos.get_sideToPlay();
    occupency = pos.get_pieces();
    rooks = pos.get_pieces(color, p_rook);
    bishops = pos.get_pieces(color, p_bishop);
    queens = pos.get_pieces(color, p_queen);
    
    
    while(rooks)
    {
        from = lsb_sq(rooks);
        //check now if the from square is absolute pinned
        rookSquares = target & sliderAttacks(from, occupency, p_rook);
        //printBitboard(rookSquares);
        while(rookSquares)
        {
             //could check here if the to square places their king in check
            to = lsb_sq(rookSquares);
            mov = make_move(from, to);
            (*moveList++).mv = mov;
            pop_lsb(&rookSquares);
        }
        pop_lsb(&rooks);
    }
    
    while(bishops)
    {
        from = lsb_sq(bishops);
        //check now if the from square is absolute pinned
        bishopSquares = target & sliderAttacks(from, occupency, p_bishop);
        
        while(bishopSquares)
        {
            //could check here if the to square places their king in check
            to = lsb_sq(bishopSquares);
            mov = make_move(from, to);
            (*moveList++).mv = mov;
            pop_lsb(&bishopSquares);
        }
        pop_lsb(&bishops);
    }
    
    while(queens)
    {
        from = lsb_sq(queens);
        //check now if the from square is absolute pinned
        queenSquares = target & (sliderAttacks(from, occupency, p_rook) | sliderAttacks(from, occupency, p_bishop));
        //printBitboard(queenSquares);
        while(queenSquares)
        {
            //could check here if the to square places their king in check
            to = lsb_sq(queenSquares);
            mov = make_move(from, to);
            (*moveList++).mv = mov;
            pop_lsb(&queenSquares);
        }
        pop_lsb(&queens);
    }
    
    
    
    
    
    return moveList;
}

void printmove (move m)
{
   /* square from ,to;
    char mv [4];
    
        from = source_sq(m);
    to = destination_sq(m);
        mv[0] = square_to_file(from) + 'A';
        mv[1] = square_to_rank(from) + '1';
        mv[2] = square_to_file(to) + 'A';
        mv[3] = square_to_rank(to) + '1';
        std::cout << mv[0] << mv[1] << mv[2] << mv[3] << "\n";
    
    */
    std::string pieceletters = "_NBRQ";
    std::string flagletters = "_recp";
    square from ,to;
    char mv [7];
    
    from = source_sq(m);
    to = destination_sq(m);
    mv[0] = square_to_file(from) + 'A';
    mv[1] = square_to_rank(from) + '1';
    mv[2] = square_to_file(to) + 'A';
    mv[3] = square_to_rank(to) + '1';
    mv[4] = ' ';
    mv[5] = pieceletters[promo_pieceType(m)] ;
    mv[6] = flagletters[move_flag(m)+1]; //for non promotions, shows up at N because the offset of piece promotion
    std::cout << mv[0] << mv[1] << mv[2] << mv[3] << mv[4] << mv[5] << mv[6]  << "\n";
}

void printMoves (candidate* start, candidate* end)
{
    std::string pieceletters = "_NBRQ";
    std::string flagletters = "_recp";
    square from ,to;
    char mv [7];
    
    while( start!= end)
    {
        from = source_sq(start-> mv);
        to = destination_sq(start -> mv);
        mv[0] = square_to_file(from) + 'A';
        mv[1] = square_to_rank(from) + '1';
        mv[2] = square_to_file(to) + 'A';
        mv[3] = square_to_rank(to) + '1';
        mv[4] = ' ';
        mv[5] = pieceletters[promo_pieceType(start->mv)] ;
        mv[6] = flagletters[move_flag(start->mv)+1]; //for non promotions, shows up at N because the offset of piece promotion
        std::cout << mv[0] << mv[1] << mv[2] << mv[3] << mv[4] << mv[5] << mv[6]  << "\n";
         start++;
    }
}


