CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -g

chess: Chess.o Board.o main.o
	$(CXX) $(CXXFLAGS) Chess.o Board.o main.o -o chess
unittest: test_chess.o Chess.o

Board.o: Game.h
Chess.o: Game.h Chess.h Prompts.h
main.o: Game.h Chess.h Prompts.h main.cpp
test_chess.o: test_chess.cpp Chess.cpp

clean:
	rm *.o chess

