// Minesweeper
// written by Russell Miller

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "minesweeper.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iosfwd>
#include <fstream>

using namespace std;

Minesweeper::Minesweeper( int rows_, int cols_, int mines_, int seed )
: minefield( NULL ), rows( rows_ ), cols( cols_ ), mines( mines_ ), numFlagged( 0 ),
correctFlagged( 0 ), numRevealed( 0 ),
firstRevealed( false ), checkSolvable( true )
{
	srand( seed );

	if ( rows < 9 )
	{
		cout << "minimum rows is 9" << endl;
		rows = 9;
	} // if
	if ( cols < 9 )
	{
		cout << "minimum cols is 9" << endl;
		cols = 9;
	} // if
	if ( mines < 10 )
	{
		cout << "minimum mines is 10" << endl;
		mines = 10;
	} // if
	// too many mines at 90%
	// stop impossible-to-solve checking at 25%
	else if ( mines > rows*cols * 0.25 )
	{
		if ( mines > rows*cols * 0.90 )
		{
			cout << "maximum mines for " << rows*cols << " squares is 90\% ("
				<< (int)( rows*cols * 0.90 ) << ")" << endl;
			mines = rows*cols * 0.90;
			cout << "number of mines reduced to " << mines << endl;
		} // if too many mines, mines reduced
		cout << "maximum mines for " << rows*cols
			<< " squares and impossible minefield checking is 25\% ("
			<< (int)( rows*cols * 0.25 ) << ")" << endl;
		checkSolvable = false;
	} // if too many mines

	createNewField();
} // Minesweeper constructor

void Minesweeper::createNewField()
{
	if ( minefield )
	{
		for ( int r = 0; r < rows; r++ )
			delete minefield[r];
		delete minefield;
	} // if already initialized

	// create minefield
	minefield = new Square*[rows];
	for ( int r = 0; r < rows; r++ )
		minefield[r] = new Square[cols];

	// initialize mines
	for ( int m = 0; m < mines; )
	{
		int r = rand() % rows;
		int c = rand() % cols;
		if ( !minefield[r][c].isMine() )
		{
			minefield[r][c].setMine();
			m++;
		} // if not Mine
	} // for initialize mines

	// set numbers
	setNumbers();
} // Minesweeper::createNewField()

void Minesweeper::setNumbers()
{
	for ( int r = 0; r < rows; r++ )
	for ( int c = 0; c < cols; c++ )
	{
		int mines = 0;
		if ( r != 0 ) // up
		{
			if ( minefield[r - 1][c].isMine() )
				mines++;
			if ( c != 0 ) // up left
			if ( minefield[r - 1][c - 1].isMine() )
				mines++;
			if ( c != cols - 1 ) // up right
			if ( minefield[r - 1][c + 1].isMine() )
				mines++;
		} // if up
		if ( c != 0 ) // left
		if ( minefield[r][c - 1].isMine() )
			mines++;
		if ( c != cols - 1 ) // right
		if ( minefield[r][c + 1].isMine() )
			mines++;
		if ( r != rows - 1 ) // down
		{
			if ( minefield[r + 1][c].isMine() )
				mines++;
			if ( c != 0 ) // down left
			if ( minefield[r + 1][c - 1].isMine() )
				mines++;
			if ( c != cols - 1 ) // down right
			if ( minefield[r + 1][c + 1].isMine() )
				mines++;
		} // if down

		minefield[r][c].setNumber( mines );
	} // for all Squares
} // Minesweeper::setNumbers()

bool Minesweeper::isSolvable() const
{
	/*
	KEY: x = not a number (can be off field); o = a number; m = mine
	case 1: 2+ 8's, 1+ is a mine && 1+ is not a mine
	case 2:
	x..x
	.mo.
	.om.
	x..x
	case 3:
	xxx x..x
	.m. xmox
	.o. x..x
	xxx
	*/
	int eightMine = 0;
	int eightNoMine = 0;
	for ( int r = 0; r < rows; r++ )
	{
		for ( int c = 0; c < cols; c++ )
		{
			if ( minefield[r][c].getNumber() == 8 )
			{
				if ( minefield[r][c].isMine() )
					eightMine++;
				else
					eightNoMine++;
			} // if case 1

			if ( minefield[r][c].isMine() )
			{
				if ( r != rows - 1 && !minefield[r + 1][c].isMine() )
				{
					if ( ( r == 0
						|| ( ( c == 0 || minefield[r - 1][c - 1].isMine() )
						&& minefield[r - 1][c].isMine()
						&& ( c == cols - 1 || minefield[r - 1][c + 1].isMine() ) ) )
						&& ( r == rows - 2
						|| ( ( c == 0 || minefield[r + 2][c - 1].isMine() )
						&& minefield[r + 2][c].isMine()
						&& ( c == cols - 1 || minefield[r + 2][c + 1].isMine() ) ) ) )
					{
						// cout << "case3Vert (" <<r<<","<<c<<")" << endl;
						return false;
					}
				} // if case3Vert
				if ( c != cols - 1 && !minefield[r][c + 1].isMine() )
				{
					if ( ( c == 0
						|| ( ( r == 0 || minefield[r - 1][c - 1].isMine() )
						&& minefield[r][c - 1].isMine()
						&& ( r == rows - 1 || minefield[r + 1][c - 1].isMine() ) ) )
						&& ( c == cols - 2
						|| ( ( r == 0 || minefield[r - 1][c + 2].isMine() )
						&& minefield[r][c + 2].isMine()
						&& ( r == rows - 1 || minefield[r + 1][c + 2].isMine() ) ) ) )
					{
						// cout << "case3Horz (" <<r<<","<<c<<")" << endl;
						return false;
					}
				} // if case3Horz
				if ( r != rows - 1 && c != cols - 1 && minefield[r + 1][c + 1].isMine()
					 && !minefield[r + 1][c].isMine() && !minefield[r][c + 1].isMine() )
				{
					if ( ( r == 0
						|| ( ( c == 0 || minefield[r - 1][c - 1].isMine() )
						&& ( c == cols - 2 || minefield[r - 1][c + 2].isMine() ) ) )
						&& ( r == rows - 2
						|| ( ( c == 0 || minefield[r + 2][c - 1].isMine() )
						&& ( c == cols - 2 || minefield[r + 2][c + 2].isMine() ) ) ) )
					{
						// cout << "case2 (" <<r<<","<<c<<")" << endl;
						return false;
					}
				} // if case2
			} // if (r,c) mine

			else // (r,c) not mine
			{
				if ( r != rows - 1 && minefield[r + 1][c].isMine() )
				{
					if ( ( r == 0
						|| ( ( c == 0 || minefield[r - 1][c - 1].isMine() )
						&& minefield[r - 1][c].isMine()
						&& ( c == cols - 1 || minefield[r - 1][c + 1].isMine() ) ) )
						&& ( r == rows - 2
						|| ( ( c == 0 || minefield[r + 2][c - 1].isMine() )
						&& minefield[r + 2][c].isMine()
						&& ( c == cols - 1 || minefield[r + 2][c + 1].isMine() ) ) ) )
					{
						// cout << "case3Vert (" <<r<<","<<c<<")" << endl;
						return false;
					}
				} // if case3Vert
				if ( c != cols - 1 && minefield[r][c + 1].isMine() )
				{
					if ( ( c == 0
						|| ( ( r == 0 || minefield[r - 1][c - 1].isMine() )
						&& minefield[r][c - 1].isMine()
						&& ( r == rows - 1 || minefield[r + 1][c - 1].isMine() ) ) )
						&& ( c == cols - 2
						|| ( ( r == 0 || minefield[r - 1][c + 2].isMine() )
						&& minefield[r][c + 2].isMine()
						&& ( r == rows - 1 || minefield[r + 1][c + 2].isMine() ) ) ) )
					{
						// cout << "case3Horz (" <<r<<","<<c<<")" << endl;
						return false;
					}
				} // if case3Horz
				if ( r != rows - 1 && c != cols - 1 && !minefield[r + 1][c + 1].isMine()
					 && minefield[r + 1][c].isMine() && minefield[r][c + 1].isMine() )
				{
					if ( ( r == 0
						|| ( ( c == 0 || minefield[r - 1][c - 1].isMine() )
						&& ( c == cols - 2 || minefield[r - 1][c + 2].isMine() ) ) )
						&& ( r == rows - 2
						|| ( ( c == 0 || minefield[r + 2][c - 1].isMine() )
						&& ( c == cols - 2 || minefield[r + 2][c + 2].isMine() ) ) ) )
					{
						// cout << "case2 (" <<r<<","<<c<<")" << endl;
						return false;
					}
				} // if case2
			} // if mine
		} // for
	} // for

	if ( eightMine && eightNoMine )
	{
		// cout << "case1" << endl;
		return false; // case 1 fail
	}

	return true;
} // isSolvable


void Minesweeper::printStats() const
{
	cout << "Flags Left:       " << mines - numFlagged << endl;
	cout << "Squares Revealed: " << numRevealed << endl;
	cout << "Percent Revealed: " << setprecision( 4 )
		<< 100 * (double)numRevealed / ( rows*cols - mines )
		<< "\%" << endl;
	cout << endl;
} // printStats()
void Minesweeper::printEndStats() const
{
	cout << "Flags Left:       " << mines - numFlagged << endl;
	cout << "Correct Flags:    " << correctFlagged << endl;
	cout << "Incorrect Flags:  " << numFlagged - correctFlagged << endl;
	cout << "Squares Revealed: " << numRevealed << endl;
	cout << "Percent Revealed: " << setprecision( 4 )
		<< 100 * (double)numRevealed / ( rows*cols - mines )
		<< "\%" << endl;

	ofstream myFile;
	myFile.open( "Record.txt" , ios_base::app);

	myFile << 100 * (double)numRevealed / ( rows*cols - mines );
	myFile << '\n';

	ofstream avr( "Avr.txt" );
	ifstream inFile( "Record.txt" );

	double num;
	double sum = 0;
	int count = 0;
	while ( inFile >> num )
	{
		sum += num;
		count++;
	}

	avr << sum / count;

	//Application.Restart();

	// cout << endl << "Game Over" << endl;
} // printStats()

void Minesweeper::printSol( int row, int col ) const
{ // for printing when gameOver()

	HANDLE  hConsole;
	hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

	cout << endl;
	int width = 2;
	int rwidth = 1;
	for ( int i = 10; i < cols; i *= 10 )
		width++;
	for ( int i = 10; i < rows; i *= 10 )
		rwidth++;
	for ( int r = -1; r < rows; r++ )
	{
		if ( r == -1 )
		for ( int i = 0; i < rwidth + 1; i++ )
			cout << " ";
		else
		{
			int wtemp = cout.width();
			cout << setw( rwidth ) << right << r << left << setw( wtemp ) << " ";
		}
		for ( int c = 0; c < cols; c++ )
		{
			if ( r == -1 )
			{
				int wtemp = cout.width();
				cout << setw( width ) << left << c << setw( wtemp );
			} // if row -1
			else
			{
				if ( r == row && c == col )
					cout << "*"; // revealed this mine
				else if ( minefield[r][c].isFlagged() )
				if ( minefield[r][c].isMine() )
				{
					SetConsoleTextAttribute( hConsole, 12 );
					cout << "+"; // flag
					SetConsoleTextAttribute( hConsole, 7 );
				}
				else
					cout << "W"; // flagged number
				else if ( minefield[r][c].isMine() )
					cout << "X"; // unflagged mine
				else if ( minefield[r][c].isRevealed() )
				{
					int num = minefield[r][c].getNumber();
					num != 0 ? cout << num : cout << " ";
				}
				//cout << minefield[r][c].getNumber(); // revealed number
				else if ( minefield[r][c].isQuestion() )
					cout << "?"; // question
				else
					cout << "-"; // unrevealed number
				for ( int i = 0; i < width - 1; i++ )
					cout << " ";
			} // not row -1
		} // for all Squares
		cout << endl;
	} // for
	cout << endl;
	printEndStats(); cout << endl;
} // Minesweeper::printSol()

void Minesweeper::print() const
{
	HANDLE  hConsole;
	hConsole = GetStdHandle( STD_OUTPUT_HANDLE );


	cout << endl; cout << endl;
	int width = 2;
	int rwidth = 1;
	for ( int i = 10; i < cols; i *= 10 )
		width++;
	for ( int i = 10; i < rows; i *= 10 )
		rwidth++;
	for ( int r = -1; r < rows; r++ )
	{
		if ( r == -1 )
		for ( int i = 0; i < rwidth + 1; i++ )
			cout << " ";
		else
		{
			int wtemp = cout.width();
			cout << setw( rwidth ) << right << r << left << setw( wtemp ) << " ";
		}
		for ( int c = 0; c < cols; c++ )
		{
			if ( r == -1 )
			{
				int wtemp = cout.width();
				cout << setw( width ) << left << c << setw( wtemp );
			} // if row -1
			else
			{
				if ( minefield[r][c].isFlagged() )
				{
					SetConsoleTextAttribute( hConsole, 12 );
					cout << "+"; // flag
					SetConsoleTextAttribute( hConsole, 7 );
				}

				else if ( minefield[r][c].isRevealed() )
				{
					int num = minefield[r][c].getNumber();
					if ( num != 0 )
					{
						cout << num; // revealed number
					}
					else
					{
						cout << " ";
					}
				}
				else if ( minefield[r][c].isQuestion() )
					cout << "?"; // question
				else
					cout << "-"; // unrevealed
				for ( int i = 0; i < width - 1; i++ )
					cout << " ";
			} // not row -1
		} // for all Squares
		cout << endl;
	} // for
	cout << endl;
	printStats();

} // Minesweeper::print()

void Minesweeper::gameOver( int r, int c, bool win )
// parameters r, c to show what mine made user lose
{ // to be called when lose
	printSol( r, c ); // do not print undiscovered numbers
	if ( win )
		cout << endl << "Congratulations!" << endl;
	else
		cout << endl << "***Game Over***" << endl;
	for ( int r = 0; r < rows; r++ )
		delete minefield[r];
	delete minefield;
	cout << endl;
	exit( 0 );
	
} // Minesweeper::gameOver()

void Minesweeper::endGame()
{
	gameOver( -1, -1, false );
} // Minesweeper::endGame()


int Minesweeper::numRows() const
{
	return rows;
}
int Minesweeper::numCols() const
{
	return cols;
}
int Minesweeper::totalMines() const
{
	return mines;
}
int Minesweeper::minesMarked() const
{
	return numFlagged;
}


int Minesweeper::revealSquare( int r, int c )
{
	if ( minefield[r][c].isFlagged() )
		return -1;

	if ( !firstRevealed )
	{
		firstRevealed = true;
		if ( checkSolvable )
		while ( minefield[r][c].isMine() || minefield[r][c].getNumber() != 0
				|| !isSolvable() )
		{
			// if( minefield[r][c].isMine() )
			//   cout << "starting on mine" << endl;
			// else if( minefield[r][c].getNumber() != 0 )
			//   cout << "starting on not 0" << endl;
			// else
			// {
			//   // cout << "not solvable" << endl;
			//   printSol( -1, -1 );
			// }
			createNewField();
		}
	} // if first Reveal

	if ( minefield[r][c].isMine() )
		gameOver( r, c, false );
	bool newReveal = false;
	if ( !minefield[r][c].isRevealed() )
	{
		numRevealed++;
		newReveal = true;
	}
	minefield[r][c].reveal();
	if ( newReveal && minefield[r][c].getNumber() == 0 )
		revealSur( r, c );

	if ( numRevealed == rows*cols - mines )
		gameOver( -1, -1, true ); // win condition

	return minefield[r][c].getNumber();
} // Minesweeper::revealSquare()

int Minesweeper::isRevealed( int r, int c ) const
{
	if ( minefield[r][c].isRevealed() )
		return minefield[r][c].getNumber();
	else
		return -1;
} // Minesweeper::isRevealed()


void Minesweeper::flagSquare( int r, int c )
{
	if ( minefield[r][c].isRevealed() )
		return;

	if ( !minefield[r][c].isFlagged() )
	{
		numFlagged++;
		if ( minefield[r][c].isMine() )
			correctFlagged++;
	}
	else
	{
		numFlagged--;
		if ( minefield[r][c].isMine() )
			correctFlagged--;
	}
	minefield[r][c].setFlagged();
} // Minesweeper::flagSquare()

bool Minesweeper::isFlagged( int r, int c ) const
{
	return minefield[r][c].isFlagged();
} // Minesweeper::isFlagged()


void Minesweeper::questionSquare( int r, int c )
{
	minefield[r][c].setQuestion();
} // Minesweeper::flagSquare()

bool Minesweeper::isQuestion( int r, int c ) const
{
	return minefield[r][c].isQuestion();
} // Minesweeper::isFlagged()


int Minesweeper::surFlags( int r, int c ) const
{
	int flags = 0;
	if ( r != 0 ) // up
	{
		if ( minefield[r - 1][c].isFlagged() )
			flags++;
		if ( c != 0 ) // up left
		if ( minefield[r - 1][c - 1].isFlagged() )
			flags++;
		if ( c != cols - 1 ) // up right
		if ( minefield[r - 1][c + 1].isFlagged() )
			flags++;
	} // if up
	if ( c != 0 ) // left
	if ( minefield[r][c - 1].isFlagged() )
		flags++;
	if ( c != cols - 1 ) // right
	if ( minefield[r][c + 1].isFlagged() )
		flags++;
	if ( r != rows - 1 ) // down
	{
		if ( minefield[r + 1][c].isFlagged() )
			flags++;
		if ( c != 0 ) // down left
		if ( minefield[r + 1][c - 1].isFlagged() )
			flags++;
		if ( c != cols - 1 ) // down right
		if ( minefield[r + 1][c + 1].isFlagged() )
			flags++;
	} // if down

	return flags;
} // Minesweeper::surFlags()

int Minesweeper::surEmpty( int r, int c ) const
{ // returns num of unflagged, unrevealed surrounding squares
	int numEmpty = 0;
	if ( r != 0 ) // up
	{
		if ( !minefield[r - 1][c].isFlagged() && !minefield[r - 1][c].isRevealed() )
			numEmpty++;
		if ( c != 0 ) // up left
		if ( !minefield[r - 1][c - 1].isFlagged() && !minefield[r - 1][c - 1].isRevealed() )
			numEmpty++;
		if ( c != cols - 1 ) // up right
		if ( !minefield[r - 1][c + 1].isFlagged() && !minefield[r - 1][c + 1].isRevealed() )
			numEmpty++;
	} // if up
	if ( c != 0 ) // left
	if ( !minefield[r][c - 1].isFlagged() && !minefield[r][c - 1].isRevealed() )
		numEmpty++;
	if ( c != cols - 1 ) // right
	if ( !minefield[r][c + 1].isFlagged() && !minefield[r][c + 1].isRevealed() )
		numEmpty++;
	if ( r != rows - 1 ) // down
	{
		if ( !minefield[r + 1][c].isFlagged() && !minefield[r + 1][c].isRevealed() )
			numEmpty++;
		if ( c != 0 ) // down left
		if ( !minefield[r + 1][c - 1].isFlagged() && !minefield[r + 1][c - 1].isRevealed() )
			numEmpty++;
		if ( c != cols - 1 ) // down right
		if ( !minefield[r + 1][c + 1].isFlagged() && !minefield[r + 1][c + 1].isRevealed() )
			numEmpty++;
	} // if down

	return numEmpty;
} // Minesweeper::surEmpty()

int Minesweeper::revealSur( int r, int c )
{
	// check if [r][c].number == [r][c].surFlags
	if ( !minefield[r][c].isRevealed()
		 || minefield[r][c].getNumber() != surFlags( r, c ) )
		 return 1; // error

	if ( r != 0 ) // up
	{
		// if( !minefield[r-1][c].isFlagged() && !minefield[r-1][c].isRevealed() )
		revealSquare( r - 1, c );
		if ( c != 0 ) // up left
			// if(!minefield[r-1][c-1].isFlagged() && !minefield[r-1][c-1].isRevealed())
			revealSquare( r - 1, c - 1 );
		if ( c != cols - 1 ) // up right
			// if(!minefield[r-1][c+1].isFlagged() && !minefield[r-1][c+1].isRevealed())
			revealSquare( r - 1, c + 1 );
	} // if up
	if ( c != 0 ) // left
		// if( !minefield[r][c-1].isFlagged() && !minefield[r][c-1].isRevealed() )
		revealSquare( r, c - 1 );
	if ( c != cols - 1 ) // right
		// if( !minefield[r][c+1].isFlagged() && !minefield[r][c+1].isRevealed() )
		revealSquare( r, c + 1 );
	if ( r != rows - 1 ) // down
	{
		// if( !minefield[r+1][c].isFlagged() && !minefield[r+1][c].isRevealed() )
		revealSquare( r + 1, c );
		if ( c != 0 ) // down left
			// if(!minefield[r+1][c-1].isFlagged() && !minefield[r+1][c-1].isRevealed())
			revealSquare( r + 1, c - 1 );
		if ( c != cols - 1 ) // down right
			// if(!minefield[r+1][c+1].isFlagged() && !minefield[r+1][c+1].isRevealed())
			revealSquare( r + 1, c + 1 );
	} // if down

	return 0;
} // Minesweeper::revealSur()