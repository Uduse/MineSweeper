MineSweeper.out: main.o MineSweeper.o square.o Solver.o
	g++ -Wall -g -o MineSweeper.out main.o MineSweeper.o square.o Solver.o

main.o: main.cpp minesweeper.h Solver.h
	g++ -Wall -g -c main.cpp

MineSweeper.o: MineSweeper.cpp minesweeper.h square.h
	g++ -Wall -g -c MineSweeper.cpp

square.o: square.cpp square.h
	g++ -Wall -g -c square.cpp

Solver.o: Solver.cpp Solver.h
	g++ -Wall -g -c Solver.cpp
clean:
	rm -f MineSweeper.out main.o MineSweeper.o square.o Solver.o core
