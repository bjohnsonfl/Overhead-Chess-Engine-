# Overhead
This is a UCI Engine I developed from the ground up. https://lichess.org/@/OverHead_BOT Challenge me to a game!

# Features
Overhead utilizes Bitboards and bitwise manipulations to represent board states and move generation. Magic Bitboards and Magic numbers are generated at startup for speedy slider attack generation. Negamax and alpha beta prunning is used for search and piece square tables and simple material difference is used for evlaution. Perft and PerftDivide are implemented to test and debug the move generator. No opening or endgame tables are used.

# Future
Overhead will benefit greatly from iterative deepening. Principal Variation, Transposition Table, move history/ordering hueristics, and null move heuristics will need to be developed. With these new additions time management can then be accmplished as currently every move is searched to a depth of 7. Aspiration windows and depth extentions will also be useful. UCI stop and ponder would be great too. Evaluation can and will always need updating!

# Requirments 
Currently, Overhead requires 64 bit and a GCC compiler due to a LSB function in bitboard.hpp. If you are a windows user, you can replace that function with _BitScanForward64 in the intrin.h library. That should be updated in the future. Simply type `make` to compile and `./Overhead` to run. `make clean` will remove the .o files. If the makefile is acting strange, make sure spaces and tabings are correct. 

# Acknowledgements 
I studied Stockfish, https://www.Chessprogramming.org, and Vice videos for inspiration.
If you would like to use my code or parts of it, please reference me. Thank you and enjoy!
