#pragma once

#include "minesweeper.h"
#include <vector>
#include <iomanip>
#include <iostream>
#include <queue>

using namespace std;

struct block
{
	int row;
	int col;

	block()
	{
		//row = 0;
		//col = 0;
	}

	block( int r, int c )
	{
		row = r;
		col = c;
	}

	void print() const
	{
		cout << "<" << setw( 2 ) << row << "," << setw( 2 ) << col << ">" << endl;
	}

};

class Solver
{
public:
	Solver();
	Solver( Minesweeper* inputGame );
	~Solver();
	void solve();

	double printWithNoGuess( double noGuess );

	double checkPercent();

	bool solveSure();

	void solveDirect();

	bool Guess();

	bool revealCorner();
	bool revealEdge();
	bool revealQuarterEdge();
	bool checkChanged( bool changedSure, queue<int> &before, queue<int> &after );

	bool doulbleClick_PushNewTODO( const block &target );

	void doubleClick( const block &target );

	bool flagSureAndDoubleClick( const block &target );

	void showFeedback( const block &target );

	int getSurFlag( const block &target );

	void queueSur( queue<block> &que, const block &target );
	void queueSurNumbered( queue<block> &que, const block &target );
	void queueNextNumbered( queue<block> &que, const block &target );
	void queueIntSur( queue<int> &que, const block &target );
	//void queueSur2( const block &target );
	void todoSur( const block &target );
	int getNum( const block &target );
	bool flagBlock( const block &target );
	int getSurEmpty( const block &target );
	void pushRevealed();

	void printTODO();
	void saveBoard( queue<int> &que );
	void solvePattern();

	void diffPattern( block* center, block* neighbor );

	void revealLessRisky();
	bool isValid( block &target );
private:
	Minesweeper* game;
	vector<block> todo;
	vector<block>::iterator itr;
	vector<block>::reverse_iterator r_itr;

	queue<block> now;
	queue<block> now2;
};

