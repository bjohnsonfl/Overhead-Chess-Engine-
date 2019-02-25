CXX = g++
CXXFLAGS = -Wall -g -std=c++11
#-std=gnu++14

objects = main.o bitboard.o moveGen.o position.o search.o types.o evaluate.o UCI.o

Overhead: $(objects)
	$(CXX) $(CXXFLAGS) -o Overhead $(objects)
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp
bitboard.o: bitboard.cpp
	$(CXX) $(CXXFLAGS) -c bitboard.cpp
moveGen.o: moveGen.cpp
	$(CXX) $(CXXFLAGS) -c moveGen.cpp
position.o: position.cpp
	$(CXX) $(CXXFLAGS) -c position.cpp
search.o: search.cpp
	$(CXX) $(CXXFLAGS) -c search.cpp
types.o: types.cpp types.hpp
	$(CXX) $(CXXFLAGS) -c types.cpp
evaluate.o: evaluate.cpp
	$(CXX) $(CXXFLAGS) -c evaluate.cpp
UCI.o: UCI.cpp
	$(CXX) $(CXXFLAGS) -c UCI.cpp
clean :
	rm Overhead $(objects)