// Minesweeper driver
// written by Russell Miller

//#include "stdafx.h"
#include <iostream>
#include "minesweeper.h"
#include "Solver.h"

using namespace std;

double percent;

Minesweeper* makeGame();
void solveMinesweeper( Minesweeper* game );

int main()
{
	//srand( time( NULL ) );

	//int times = 5;
	//for ( int i = 0; i < times; i++ )
	//{
	Minesweeper* game = makeGame();
	solveMinesweeper( game );
	//cin.get();
	//}

	//cout << "***Avecrage Reveal***" << endl;
	//cout << percent / (double)times;
} // main()

Minesweeper* makeGame()
{
	int choice = 0, seed;
	cout << "Seed for Random Number Generator: ";
	cin >> seed; //CHANGED
	cout << "Minesweeper Menu:" << endl;
	cout << "1. Advanced (30x16, 99 mines) (default)" << endl
		<< "2. Intermediate (16x16, 40 mines)" << endl
		<< "3. Beginner (9x9, 10 mines)" << endl
		<< "4. Custom" << endl
		<< "Choice: ";

	cin >> choice; //CHANGED

	//====================================//
	//  MY Init
	//====================================//
	//choice = 1;
	//seed = rand() % 65535;
	//seed = 20200;
	cout << "seed: " << seed << endl;

	// if( choice == 1 )
	//   return new Minesweeper(30,16,99,seed);
	if ( choice == 2 )
		return new Minesweeper( 16, 16, 40, seed );
	if ( choice == 3 )
		return new Minesweeper( 9, 9, 10, seed );
	if ( choice == 4 )
	{
		int rows, cols, mines;
		cout << "Specifications?" << endl
			<< "(format: rows cols mines)" << endl;
		//cin >> rows >> cols >> mines;

		rows = 16;
		cols = 30;
		mines = 99;

		// cout << rows << "," << cols << "," << mines << endl;
		return new Minesweeper( rows, cols, mines, seed );
	} // if custom
	return new Minesweeper( 30, 16, 99, seed );
} // makeGame()


void solveMinesweeper( Minesweeper* game )
{
	// MUST USE THIS FIRST LINE
	game->revealSquare( game->numRows() / 2, game->numCols() / 2 );
	// MUST USE THIS FIRST LINE

	Solver mySolver( game );
	mySolver.solve();

	//game->revealSquare(15,10);
	//game->revealSquare(5,7);
	//game->revealSquare(20,8);
	//game->revealSquare(18,2);
	//game->revealSquare(25,5);
	//game->revealSquare(25,6);
	//game->revealSquare(25,7);
	//game->revealSquare(26,6);
	//game->revealSquare(26,7);
	//game->print();
	// Create solver here!

	game->endGame();
} // solveMinesweeper()




