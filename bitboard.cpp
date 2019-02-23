//
//  bitboard.cpp
//  Chess Engine
//
//  Created by Blake Johnson on 12/12/18.
//  Copyright © 2018 Blake Johnson. All rights reserved.
//

#include "bitboard.hpp"

bitboard squareBitMask [numOfSq];
bitboard fileBitMask [numOfFile];
bitboard rankBitMask [numOfRank];
bitboard diagonalBitMask [numOfSq];
bitboard antiDiagonalBitMask [numOfSq];

bitboard clearFile [numOfFile];
bitboard clearRank [numOfRank];

bitboard openBoardAttacks[numOfPieceType][numOfSq];
bitboard openBoardPawnAttacks[2][64];
bitboard inBetweenSqBitMask [numOfSq][numOfSq];
bitboard lineSqBitMask [numOfSq][numOfSq];

bitboard magicRookMask [numOfSq];
bitboard magicRookNumbers [numOfSq];
bitboard * magicRookIndex [64];
bitboard magicRookAttacks [102400];
bitboard magicBishopMask [numOfSq];
bitboard magicBishopNumbers [numOfSq];
bitboard * magicBishopIndex [64];
bitboard magicBishopAttacks [5248];//5248


uint64_t randomU64 ()
{
    uint64_t val1, val2, val3, val4;
    val1 = uint64_t(rand() & 0xFFFF);
    val2 = uint64_t(rand() & 0xFFFF) << 16;
    val3 = uint64_t(rand() & 0xFFFF) << 32;
    val4 = uint64_t(rand() & 0xFFFF) << 48;
    
    
    return val1 | val2 | val3 | val4;

}
uint64_t randomU64_fewbits () {
    
    return randomU64() & randomU64() & randomU64();
}

//credit to Torn Romstad
int popcount(bitboard b) {
    int r;
    for(r = 0; b; r++, b &= b - 1);
    return r;
}

void bitboards::init(){

  // std::cout << "Generating Bitboard Masks... \n";
    for(int i=0; i<numOfSq; i++)
    {
          squareBitMask[i] = (1ULL << i);
    }
    
    //diagonal and antiDiagonal bit masks
    for(int i=0; i<numOfSq; i++)
    {
        
        
        diagonalBitMask[i]  = diagMasks(i, 0) ;//& clearEdges;
        
       
        antiDiagonalBitMask[i] = antidiagMasks(i, 0);// & clearEdges;
        
        
    }
    
    for( int i = 0; i < numOfFile; i++)
    {
        fileBitMask[i] = fileA << i;
        clearFile[i] = ~fileBitMask[i];
    }
    
    for( int i = 0; i < numOfRank; i++)
    {
        rankBitMask[i] = rank1 << (8*i);
        clearRank[i] = ~rankBitMask[i];
    }
    // std::cout << "Generating Piece Attacks... \n";
     allPawnAttacks();
     allKingAttacks();
     allKnightAttacks();
     allBishopAttacks();
     allRookAttacks();
     allQueenAttacks();
     //std::cout << "Generating Magic Bishop Boards... \n";
     initMagicBishop();
     //std::cout << "Generating Magic Rook Boards... \n";
     initMagicRook();
     inBetweenBitMask();
     //std::cout << "Generating Bitboards Complete... \n";
}

bitboard diagMasks (int sq, bitboard occupied){
    int iter = sq;
   bitboard bb = squareBitMask[sq];
    
    //Diagonal
    //north east
    bitboard diag = bb;
    while( (square_to_rank(square(iter)) < numOfRank-1) && (square_to_file(square(iter)) < numOfFile-1) )
    {
        
        iter += noEa;
        diag |= (diag << noEa);
        if( squareBitMask[iter] & occupied) break;
    }
    
    //south west
    iter = sq;
    while( (square_to_rank(square(iter)) >= 0) && (square_to_file(square(iter)) > 0) && iter >= 0 )
    {
        iter +=soWe;
        diag |= (diag >> -soWe);
        if( squareBitMask[iter] & occupied) break;
    }
    
    //diagonalBitMask[sq] = diag;
    
    return diag;
}
bitboard antidiagMasks (int sq, bitboard occupied){
    
    bitboard antiDiag = squareBitMask[sq];
    int iter = sq;
    while( (square_to_rank(square(iter)) < numOfRank-1) && (square_to_file(square(iter)) > 0) )
    {
        iter += noWe;
        antiDiag |= (antiDiag << noWe);
        if( squareBitMask[iter] & occupied) break;
    }
    
    //south east
    iter = sq;
    while( (square_to_rank(square(iter)) >= 0) && (square_to_file(square(iter)) < numOfFile-1) )
    {
        iter += soEa;
        antiDiag |= (antiDiag >> -soEa);
        if( squareBitMask[iter] & occupied) break;
        
    }
    
    return antiDiag;
}

bitboard vertMasks (int sq, bitboard occupied){
    int iter = sq;
    bitboard bb = squareBitMask[sq];
    
    //vertical
    //north
    bitboard vert = bb;
    while( square_to_rank(square(iter)) < numOfRank-1 )
    {
        
        iter += nort;
        vert |= (vert << nort);
        if( squareBitMask[iter] & occupied) break;
    }
    
    //south
    iter = sq;
    while( square_to_rank(square(iter)) > 0)
    {
        iter +=south;
        vert |= (vert >> -south);
        if( squareBitMask[iter] & occupied) break;
    }
    
    return vert;
}


bitboard horizMasks (int sq, bitboard occupied){
    uint iter = sq;
    bitboard bb = squareBitMask[sq];
    
    //horizontal
    //east
    bitboard horiz = bb;
    while( square_to_file(square(iter)) < numOfFile-1 )
    {
        
        iter += east;
        horiz |= (horiz << east);
        if( squareBitMask[iter] & occupied) break;
    }
    
    //west
    iter = sq;
    while( square_to_file(square(iter)) > 0)
    {
        iter +=west;
        horiz |= (horiz >> -west);
        if( squareBitMask[iter] & occupied) break;
    }
    return horiz;
}





void allPawnAttacks(){
    bitboard pawnA = 0ULL, pawnH = 0ULL, pawnAttacks = 0ULL;
    square sq = a1;
    
    for(rank rk = rank_1; rk<numOfRank-1; rk++)
    {
        for(file f = file_a; f<numOfFile; f++)
        {
            sq = rankFile_to_sq(rk, f);
            pawnA = squareBitMask[sq] & clearFile[file_a];
            pawnH = squareBitMask[sq] & clearFile[file_h];
            
            pawnAttacks |= (pawnA << noWe);
            pawnAttacks |= (pawnH << noEa);
            
            //printBitboard(pawnAttacks);
            openBoardPawnAttacks[0][sq] = pawnAttacks;
            pawnAttacks = 0ULL;
        }
    }
    
    for(rank rk = rank_8; rk > rank_1; rk--)
    {
        for(file f = file_a; f<numOfFile; f++)
        {
            sq = rankFile_to_sq(rk, f);
            pawnA = squareBitMask[sq] & clearFile[file_a];
            pawnH = squareBitMask[sq] & clearFile[file_h];
            
            pawnAttacks |= (pawnA >> -soWe);
            pawnAttacks |= (pawnH >> -soEa);
            
            //printBitboard(pawnAttacks);
            openBoardPawnAttacks[1][sq] = pawnAttacks;
            pawnAttacks = 0ULL;
        }
    }
}
void allKingAttacks(){
    bitboard kingA = 0ULL, kingH = 0ULL, kingMoves = 0ULL, king = 0ULL;
    square sq = a1;
    for(rank rk = rank_1; rk<numOfRank; rk++)
    {
        for(file f = file_a; f<numOfFile; f++)
        {
            sq = rankFile_to_sq(rk, f);
            kingA = squareBitMask[sq] & clearFile[file_a];
            kingH = squareBitMask[sq] & clearFile[file_h];
            king  = squareBitMask[sq];
            
            kingMoves |= (kingA << noWe);
            kingMoves |= (king  << nort);
            kingMoves |= (kingH << noEa);
            kingMoves |= (kingH << east);
            kingMoves |= (kingH >> -soEa);
            kingMoves |= (king  >> -south);
            kingMoves |= (kingA >> -soWe);
            kingMoves |= (kingA >> -west);
            
           // printBitboard(kingMoves);
            openBoardAttacks[p_king][sq] = kingMoves;
            kingMoves = 0ULL;
        }
        
    }
}
void allKnightAttacks(){
    bitboard knightA = 0ULL, knightB = 0ULL, knightG = 0ULL, knightH = 0ULL;
    bitboard knightMoves = 0ULL, knight = 0ULL;
    square sq = a1;
    for(rank rk = rank_1; rk<numOfRank; rk++)
    {
        for(file f = file_a; f<numOfFile; f++)
        {
            sq = rankFile_to_sq(rk, f);
            knightA = squareBitMask[sq] & clearFile[file_a];
            knightB = squareBitMask[sq] & clearFile[file_b];
            knightG = squareBitMask[sq] & clearFile[file_g];
            knightH = squareBitMask[sq] & clearFile[file_h];
            knight  = squareBitMask[sq];
            
            knightMoves |= ((knightA & knightB) << noWeWe);
            knightMoves |= (knightA  << noNoWe);
            knightMoves |= (knightH << noNoEa);
            knightMoves |= ((knightG & knightH) << noEaEa);
            knightMoves |= ((knightG & knightH) >> -soEaEa);
            knightMoves |= (knightH  >> -soSoEa);
            knightMoves |= (knightA >> -soSoWe);
            knightMoves |= ((knightA & knightB) >> -soWeWe);
            
           // printBitboard(knightMoves);
            openBoardAttacks[p_knight][sq] = knightMoves;
            knightMoves = 0ULL;
        }
        
    }
}
void allBishopAttacks(){
    for(square sq= a1; sq<numOfSq; sq++){
     
        magicBishopMask[sq] = clearEdges &( squareBitMask[sq] ^(diagonalBitMask[sq] | antiDiagonalBitMask[sq]));
        openBoardAttacks[p_bishop][sq] =  squareBitMask[sq] ^(diagonalBitMask[sq] | antiDiagonalBitMask[sq]);
       // printBitboard(openBoardAttacks[p_bishop][sq]);
    }
}
void allRookAttacks(){
    bitboard rookMoves = 0ULL;
    square sq = a1;
    for(rank rk = rank_1; rk<numOfRank; rk++)
    {
        for(file f = file_a; f<numOfFile; f++)
        {
            sq = rankFile_to_sq(rk, f);
            rookMoves = fileBitMask[f] | rankBitMask[rk];
            openBoardAttacks[p_rook][sq] = rookMoves ^ squareBitMask[sq];
           
            if ( (sq == a1) || (sq == a8) || (sq == h1) || (sq == h8) )
            {
                magicRookMask[sq] = (rookMoves & clearCorners);
            }
            else if(rk == rank_1)
                magicRookMask[sq] = squareBitMask[sq] ^ (rookMoves & (clearEdges | (rank1 & clearCorners)));
            else if (rk == rank_8)
                magicRookMask[sq] = squareBitMask[sq] ^ (rookMoves & (clearEdges | (rank8 & clearCorners)));
            else if (f == file_a)
                magicRookMask[sq] = squareBitMask[sq] ^ (rookMoves & (clearEdges | (fileA & clearCorners)));
            else if (f == file_h)
                magicRookMask[sq] = squareBitMask[sq] ^ (rookMoves & (clearEdges | (fileH & clearCorners)));
            else
            {
                magicRookMask[sq] = squareBitMask[sq] ^ (rookMoves & clearEdges );
            }
                
            //printBitboard( openBoardAttacks[p_rook][sq]);
            //printBitboard(magicRookMask[sq]);
            rookMoves = 0ULL;
            
        }
    }
}
void allQueenAttacks(){
    for(square sq= a1; sq<numOfSq; sq++){
        
        openBoardAttacks[p_queen][sq] = openBoardAttacks[p_bishop][sq] | openBoardAttacks[p_rook][sq] ;
       // printBitboard(openBoardAttacks[p_queen][sq]);
    }
}
void inBetweenBitMask(){
    //credit to stockfish
    for(square sq1= a1; sq1<numOfSq; sq1++){
        for(square sq2= a1; sq2<numOfSq; sq2++){
            if(!(openBoardAttacks[p_rook][sq1] & squareBitMask[sq2])) continue;
            
            inBetweenSqBitMask[sq1][sq2] = sliderAttacks(sq1, squareBitMask[sq2], p_rook) & sliderAttacks(sq2, squareBitMask[sq1], p_rook);
            lineSqBitMask[sq1][sq2] = (sliderAttacks(sq1, 0, p_rook) & sliderAttacks(sq2, 0, p_rook)) | squareBitMask[sq1] |squareBitMask[sq2] ;
           
            
        }
        for(square sq2= a1; sq2<numOfSq; sq2++){
            
            if(!(openBoardAttacks[p_bishop][sq1] & squareBitMask[sq2])) continue;
            inBetweenSqBitMask[sq1][sq2] = sliderAttacks(sq1, squareBitMask[sq2], p_bishop) & sliderAttacks(sq2, squareBitMask[sq1], p_bishop);
            lineSqBitMask[sq1][sq2] = (sliderAttacks(sq1, 0, p_bishop) & sliderAttacks(sq2, 0, p_bishop)) | squareBitMask[sq1] |squareBitMask[sq2] ;
                
        }
    }
}

//ONLY USE FOR ROOK OR BISHOP. UNION ROOK AND BISHOP FUNC CALLS FOR QUEEN
bitboard sliderAttacks (square sq, bitboard occupency, pieceType pt)
{
    bitboard mask, magic, index;
    int shift =0;
    if(pt == p_rook) {
        mask = magicRookMask[sq];
        magic = magicRookNumbers[sq];
        shift = rookShifts[sq];
        index = magicIndex(mask&occupency, magic, shift);
        return *(magicRookIndex[sq] + index);
        
    }
    else{
        mask = magicBishopMask[sq];
        magic = magicBishopNumbers[sq];
        shift = bishopShifts[sq];
        index = magicIndex(mask&occupency, magic, shift);
        return *(magicBishopIndex[sq]+index);
    }
}


unsigned magicIndex (bitboard blockers, bitboard magic, int shift)
{
    return (blockers * magic) >> (64 - shift);
}
void initMagicRook ()
{
    bitboard blockers[4096], attackers[4096], b, magic;
    bitboard used[4096];
    int size = 0;
    bitboard * idx;
    for ( square sq = a1; sq < numOfSq; sq++)
    {
        magicRookIndex[sq] = sq == a1 ? magicRookAttacks : magicRookIndex[sq - 1] + (size );
        b = 0;
        size = 0;
        do {
            
            blockers[size] = b;
            attackers[size] = squareBitMask[sq]^(horizMasks(sq, b) | vertMasks(sq, b));
            size++;
            b = (b - magicRookMask[sq]) & magicRookMask[sq];
        } while(b);
        
        int n = rookShifts [sq];
        int fail = 0;
        unsigned index = 0;
        
        for(int k = 0; k < 1000000000; k++) {
            
            magic = randomU64_fewbits();
            if(popcount((magicRookMask[sq] * magic) & 0xFF00000000000000ULL) < 6) continue;
            // std::cout<< "k: " << k <<" sq: "<<sq << "\n";
            for(int i = 0; i < 4096; i++) used[i] = 0;
            fail = 0;
            for(int i = 0; !fail && i < size; i++) {
                index = magicIndex(blockers[i], magic, n);
                if(used[index] == 0){
                    used[index] = attackers[i];
                }
                else if(used[index] != attackers[i]){
                    fail = 1;
                }
            }
            if(!fail) {
                // return magic;
               // std::cout<< "Rook magic: " << magic << " sq: " << sq  << "\n";
                idx = magicRookIndex[sq];
                for(int i = 0; i < size; i++) {
                    idx[i] = used[i];
                }
                magicRookNumbers[sq] = magic;
                /*////////////////
                //TEST
                b = 0;
                do {
                    
                    // printBitboard(b);
                    //  printBitboard( *(magicRookIndex[sq] + magicIndex(b, magic, rookShifts[sq])));
                    // printBitboard(squareBitMask[sq]^(vertMasks(sq, b) | horizMasks(sq, b)));
                    if(( *(magicRookIndex[sq] + magicIndex(b, magic, rookShifts[sq]))) != (squareBitMask[sq]^(horizMasks(sq, b) | vertMasks(sq, b)))) std::cout << "MAGIC DOESNT WORK \n";
                    
                    b = (b - magicRookMask[sq]) & magicRookMask[sq];
                } while(b);
                ///////////////////
                //////////////////
                 */
                break;
            }
        }
    }
}


void initMagicBishop ()
{
    bitboard blockers[4096], attackers[4096], b, magic;
    bitboard used[4096];
    int size = 0;
    bitboard * idx;
    for ( square sq = a1; sq < numOfSq; sq++)
    {
        magicBishopIndex[sq] = sq == a1 ? magicBishopAttacks : magicBishopIndex[sq - 1] + (size );
        b = 0;
        size = 0;
        do {
            
            blockers[size] = b;
            attackers[size] = squareBitMask[sq]^(diagMasks(sq, b) | antidiagMasks(sq, b));
            size++;
            b = (b - magicBishopMask[sq]) & magicBishopMask[sq];
        } while(b);
        
        int n = bishopShifts [sq];
        int fail = 0;
        unsigned index = 0;
        
        for(int k = 0; k < 1000000000; k++) {
            
            magic = randomU64_fewbits();
            if(popcount((magicBishopMask[sq] * magic) & 0xFF00000000000000ULL) < 6) continue;
           // std::cout<< "k: " << k <<" sq: "<<sq << "\n";
            for(int i = 0; i < 4096; i++) used[i] = 0;
            fail = 0;
            for(int i = 0; !fail && i < size; i++) {
                index = magicIndex(blockers[i], magic, n);
                if(used[index] == 0){
                    used[index] = attackers[i];
                }
                else if(used[index] != attackers[i]){
                    fail = 1;
                }
            }
            if(!fail) {
               // return magic;
              //  std::cout<< "magic: " << magic << " sq: " << sq  << "\n";
              idx = magicBishopIndex[sq];
                for(int i = 0; i < size; i++) {
                    idx[i] = used[i];
                }
                magicBishopNumbers[sq] = magic;
                /*////////////////
                //TEST
               b = 0;
                do {
                    
                   // printBitboard(b);
                  //  printBitboard( *(magicBishopIndex[sq] + magicIndex(b, magic, bishopShifts[sq])));
                   // printBitboard(squareBitMask[sq]^(diagMasks(sq, b) | antidiagMasks(sq, b)));
                    if(( *(magicBishopIndex[sq] + magicIndex(b, magic, bishopShifts[sq]))) != (squareBitMask[sq]^(diagMasks(sq, b) | antidiagMasks(sq, b)))) std::cout << "MAGIC DOESNT WORK \n";
               
                    b = (b - magicBishopMask[sq]) & magicBishopMask[sq];
                } while(b);
                ///////////////////
                //////////////////
                 */
                break;
            }
            }
    }
}

void printBitboard(bitboard board)
{
    std::cout << "---------------\n";
    for(rank r = rank_8; r>=0; r--)
    {
        for(file f = file_a; f<numOfFile; f++)
        {
            if(board & squareBitMask[rankFile_to_sq(r, f)]) std::cout << "# ";
            else std::cout << "- ";
        }
        std::cout <<"\n";
    }
    std::cout << "---------------\n";
}
