//#include "stdafx.h"
#include "Solver.h"

#define SAFETY_FIRST false
//#define SAFETY_FIRST true

#define CORNER_RISK_FACTOR 0.8
#define EDGE_RISK_FACTOR 0.6
#define Quart_RISK_FACTOR 0.4
bool outPutPattern = true;
bool guessed = false;

Solver::Solver()
{
}

Solver::Solver( Minesweeper* inputGame )
{
	game = inputGame;
}

Solver::~Solver()
{
}

void Solver::solve()
{
	//stk.push( block( 1, 1 ) );

	//pushRevealed();
	//printTODO();

	//game->print();

	bool changedGuess = false;
	int counter = 0;
	double noGuess = 0;
	double withGuess = 0;

	do
	{
		queue<int> before;
		queue<int> after;

		saveBoard( before );

		solveSure();

		//printWithNoGuess( noGuess );

		if ( !SAFETY_FIRST )
		{
			Guess();
			withGuess = checkPercent();
			cout << "Guess:  " << withGuess << endl;
			cout << "Guess Effect: " << withGuess - noGuess << endl; cout << endl;
		}

		saveBoard( after );

		changedGuess = false;
		changedGuess = checkChanged( changedGuess, before, after );

		counter++;
		//cout << endl;
		//game->print();
	} while ( changedGuess );

	//checkPercent();
	//cout << "counter: " << counter << endl;

	//game->print();
	//cout << "Guess Effect: " << withGuess - noGuess << endl; cout << endl;

	return;
}

void Solver::pushRevealed()
{
	for ( int row = 0; row < game->numRows(); row++ )
	{
		for ( int col = 0; col < game->numCols(); col++ )
		{
			if ( game->isRevealed( row, col ) > 0 )
			{
				todo.push_back( block( row, col ) );
			}
		}
	}
}

bool Solver::flagSureAndDoubleClick( const block &target )
{
	doubleClick( target );
	if ( getNum( target ) == getSurEmpty( target ) + getSurFlag( target ) )
	{
		queueSur( now, target );

		while ( !now.empty() )
		{
			// if a new flag is set, double click all sur
			if ( flagBlock( now.front() ) )
			{
				//showFeedback( now.front() );

				queueSur( now2, now.front() );
				while ( !now2.empty() )
				{
					bool changed = doulbleClick_PushNewTODO( now2.front() );
					if ( changed )
					{
						//showFeedback( now.front() );
					}
					now2.pop();
				}
			}

			now.pop();
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool Solver::doulbleClick_PushNewTODO( const block &target )
{
	bool changed = false;

	if ( getNum( target ) > 0 )
	{
		//target.print();

		queue<block> before;
		queue<block> after;

		queueSur( before, target );
		doubleClick( target );
		queueSur( after, target );

		//T1
		// if changed, push to TODO
		while ( !before.empty() )
		{
			if ( getNum( before.front() ) != getNum( after.front() ) )
			{
				todo.push_back( after.front() );
				changed = true;
			}
			before.pop();
			after.pop();
		}

		//T2
		//while ( !after.empty() )
		//{
		//	flagSureAndDoubleClick( after.front() );
		//	after.pop();
		//}
	}

	return changed;
}

////   Sure    ///////////////////////////

bool Solver::solveSure()
{
	bool changedSure = false;
	do
	{
		queue<int> before;
		queue<int> after;
		saveBoard( before );

		solvePattern();
		solveDirect();

		saveBoard( after );
		//game->print();

		// check if changed
		changedSure = checkChanged( changedSure, before, after );
	} while ( changedSure );
	return changedSure;
}

void Solver::solveDirect()
{
	pushRevealed();
	while ( !todo.empty() )
	{
		flagSureAndDoubleClick( todo.back() );
		todo.pop_back();
	}
}

void Solver::solvePattern()
{
	pushRevealed();
	while ( !todo.empty() )
	{
		block* center = &todo.back();
		//====================================//
		//  Check 121
		//====================================//
		// if equals 2 and not on edge
		if ( getNum( *center ) == 2
			 && getSurFlag( *center ) == 0
			 && ( center->row > 0
			 && center->row < game->numRows() - 1 )
			 && ( center->col > 0 && center->col < game->numCols() - 1 ) )
		{
			block blocks[3][3];
			blocks[0][0] = block( center->row - 1, center->col - 1 );
			blocks[0][1] = block( center->row - 1, center->col );
			blocks[0][2] = block( center->row - 1, center->col + 1 );
			blocks[1][0] = block( center->row, center->col - 1 );
			blocks[1][2] = block( center->row, center->col + 1 );
			blocks[2][0] = block( center->row + 1, center->col - 1 );
			blocks[2][1] = block( center->row + 1, center->col );
			blocks[2][2] = block( center->row + 1, center->col + 1 );

			if ( getNum( blocks[1][0] ) == 1
				 && getNum( blocks[1][2] ) == 1 )
			{
				//
				// 1 2 1
				//
				if ( getNum( blocks[0][0] ) == -1
					 && getNum( blocks[0][1] ) == -1
					 && getNum( blocks[0][2] ) == -1 )
				{
					// * * *
					// 1 2 1
					//
					flagBlock( blocks[0][0] );
					flagBlock( blocks[0][2] );
					if ( outPutPattern )
					{
						cout << ">> Pattern 121"; cout << endl;
					}
				}
				else if ( getNum( blocks[2][0] ) == -1
						  && getNum( blocks[2][1] ) == -1
						  && getNum( blocks[2][2] ) == -1 )
				{
					//
					// 1 2 1
					// * * *
					flagBlock( blocks[2][0] );
					flagBlock( blocks[2][2] );
					if ( outPutPattern )
					{
						cout << ">> Pattern 121"; cout << endl;
					}
				}
			}
			else if ( getNum( blocks[0][1] ) == 1
					  && getNum( blocks[2][1] ) == 1 )
			{
				//   1
				//   2
				//   1

				if ( getNum( blocks[0][0] ) == -1
					 && getNum( blocks[1][0] ) == -1
					 && getNum( blocks[2][0] ) == -1 )
				{
					// * 1
					// * 2
					// * 1
					flagBlock( blocks[0][0] );
					flagBlock( blocks[2][0] );
					if ( outPutPattern )
					{
						cout << ">> Pattern 121"; cout << endl;
					}
				}
				else if ( getNum( blocks[0][2] ) == -1
						  && getNum( blocks[0][2] ) == -1
						  && getNum( blocks[0][2] ) == -1 )
				{
					//   1 *
					//   2 *
					//   1 *
					flagBlock( blocks[0][2] );
					flagBlock( blocks[2][2] );
					if ( outPutPattern )
					{
						cout << ">> Pattern 121"; cout << endl;
					}
				}
			}

			//queue<block> que121;

			//queueSurNumbered( que121, *center );
		}

		queue<block> que;
		queueNextNumbered( que, *center );

		while ( !que.empty() )
		{
			block* neighbor = &que.front();
			que.pop();

			diffPattern( center, neighbor );
		}

		todo.pop_back();
	}
}

void Solver::diffPattern( block* center, block* neighbor )
{
	int diff = getNum( *center ) - getNum( *neighbor );

	if ( diff > 1 )
	{
		// the larger one as center
		if ( getNum( *center ) < getNum( *neighbor ) )
		{
			swap( center, neighbor );
		}

		// Locate target blocks
		block blocks[3];
		blocks[0] = block( 2 * center->row - neighbor->row + center->col - neighbor->col,
						   2 * center->col - neighbor->col + center->row - neighbor->row );
		blocks[1] = block( 2 * center->row - neighbor->row,
						   2 * center->col - neighbor->col );
		blocks[2] = block( 2 * center->row - neighbor->row - ( center->col - neighbor->col ),
						   2 * center->col - neighbor->col - ( center->row - neighbor->row ) );

		int countFlagged = 0;
		switch ( diff )
		{
		case 2:
			//====================================//
			// * - - -
			// * 3 1 -
			// * - - -
			//====================================//

			// If two flagged or two unrevealed, solve pattern
			if ( false )
			{
			}
			break;
		case 3:
			//====================================//
			// * - - -
			// * 4 1 -
			// * - - -
			//====================================//
			//game->print();
			if ( flagBlock( blocks[0] ) )
				countFlagged++;
			if ( flagBlock( blocks[1] ) )
				countFlagged++;
			if ( flagBlock( blocks[2] ) )
				countFlagged++;
			if ( countFlagged > 0 )
			{
				cout << ">> Pattern Diff 3"; cout << endl;
			}

			break;

		default:
			break;
		}
	}
}

////   Guess    ///////////////////////////

bool Solver::Guess()
{
	//game->print();

	//cout << "Before" << endl;
	if ( checkPercent() < CORNER_RISK_FACTOR &&  !revealCorner() )
	{
		if ( checkPercent() < EDGE_RISK_FACTOR && !revealEdge() )
		{
			if ( checkPercent() < Quart_RISK_FACTOR && !revealQuarterEdge() )
			{
				revealLessRisky();
			}
			else
			{
				cout << ">> Quarter" << endl;
			}
		}
		else
		{
			cout << ">> Edge" << endl;
		}
	}
	else
	{
		cout << ">> Corner" << endl;
	}
	//cout << endl; cout << endl;
	//game->print();

	return false;
}

bool Solver::revealCorner()
{
	block corner[4];
	corner[0] = block( 0, 0 );
	corner[1] = block( 0, game->numCols() - 1 );
	corner[2] = block( game->numRows() - 1, 0 );
	corner[3] = block( game->numRows() - 1, game->numCols() - 1 );

	for ( int index = 0; index < 4; index++ )
	{
		block target = corner[index];
		if ( getNum( target ) == -1 && !game->isFlagged( target.row, target.col ) )
		{
			game->revealSquare( target.row, corner[index].col );
			return true;
		}
	}
	return false;
}

bool Solver::revealEdge()
{
	block edge[8];
	bool doubleEdge = false;
	int row = game->numRows() / 2;
	int col = game->numCols() / 2;

	edge[0] = block( row, 0 );
	edge[1] = block( row, game->numCols() - 1 );
	edge[2] = block( 0, col );
	edge[3] = block( game->numRows() - 1, col );
	edge[4] = block( row + 1, 0 );
	edge[5] = block( row + 1, game->numCols() - 1 );
	edge[6] = block( 0, col + 1 );
	edge[7] = block( game->numRows() - 1, col + 1 );

	for ( int index = 0; index < 4 || index < 8 && doubleEdge; index++ )
	{
		block target = edge[index];
		if ( getNum( target ) == -1 && !game->isFlagged( target.row, target.col ) && getSurEmpty( target ) == 5 )
		{
			game->revealSquare( target.row, edge[index].col );
			//game->print();
			return true;
		}
	}
	return false;
}

bool Solver::revealQuarterEdge()
{
	block edge[8];
	int row = (int)( (double)game->numRows() / 4.0 );
	int col = (int)( (double)game->numCols() / 4.0 );
	int row2 = (int)( 3 * (double)game->numRows() / 4.0 );
	int col2 = (int)( 3 * (double)game->numCols() / 4.0 );

	edge[0] = block( row, 0 );
	edge[1] = block( row2, 0 );
	edge[2] = block( row, game->numCols() - 1 );
	edge[3] = block( row2, game->numCols() - 1 );

	edge[4] = block( 0, col );
	edge[5] = block( 0, col2 );
	edge[6] = block( game->numRows() - 1, col );
	edge[7] = block( game->numRows() - 1, col2 );

	for ( int index = 0; index < 8; index++ )
	{
		block target = edge[index];
		if ( getNum( target ) == -1 && !game->isFlagged( target.row, target.col ) && getSurEmpty( target ) == 5 )
		{
			game->revealSquare( target.row, edge[index].col );
			//game->print();
			return true;
		}
	}
	return false;
}
void Solver::revealLessRisky()
{
}

////   Helper Function    ///////////////////////////
int Solver::getNum( const block &target )
{
	return game->isRevealed( target.row, target.col );
}

int Solver::getSurEmpty( const block &target )
{
	return game->surEmpty( target.row, target.col );
}

int Solver::getSurFlag( const block &target )
{
	return game->surFlags( target.row, target.col );
}

bool Solver::flagBlock( const block &target )
{
	/*int r = target.row;
	int c = target.col;
	if ( r < 0 || r > game->numRows() - 1 || c < 0 || c > game->numCols() - 1 )
	{
	return false;
	}*/
	if ( !game->isFlagged( target.row, target.col ) )
	{
		game->flagSquare( target.row, target.col );
		return game->isFlagged( target.row, target.col );
	}
	else
	{
		return false;
	}
}

void Solver::todoSur( const block &target )
{
	for ( int i = -1; i < 2; i++ )
	{
		for ( int j = -1; j < 2; j++ )
		{
			// if is a surrending unit
			if ( !( i == 0 && j == 0 ) )
			{
				int row = target.row + i;
				int col = target.col + j;
				// valid position
				if ( ( row >= 0 && row < game->numRows() ) && ( col >= 0 && game->numCols() ) )
				{
					todo.push_back( block( row, col ) );
				}
			}
		}
	}
}

void Solver::printTODO()
{
	cout << "TODO: " << endl;
	for ( r_itr = todo.rbegin(); r_itr != todo.rend(); r_itr++ )
	{
		r_itr->print();
	}
}

void Solver::queueSur( queue<block> &que, const block &target )
{
	for ( int i = -1; i < 2; i++ )
	{
		for ( int j = -1; j < 2; j++ )
		{
			// if is a surrending unit
			if ( !( i == 0 && j == 0 ) )
			{
				int row = target.row + i;
				int col = target.col + j;
				// valid position
				if ( ( row >= 0 && row < game->numRows() ) && ( col >= 0 && col < game->numCols() ) )
				{
					que.push( block( row, col ) );
				}
			}
		}
	}
}

void Solver::queueSurNumbered( queue<block> &que, const block &target )
{
	for ( int i = -1; i < 2; i++ )
	{
		for ( int j = -1; j < 2; j++ )
		{
			// if is a surrending unit
			if ( !( i == 0 && j == 0 ) )
			{
				int row = target.row + i;
				int col = target.col + j;
				// valid position
				if ( ( row >= 0 && row < game->numRows() ) && ( col >= 0 && game->numCols() ) )
				{
					if ( getNum( block( row, col ) ) > 0 )
					{
						que.push( block( row, col ) );
					}
				}
			}
		}
	}
}

void Solver::queueNextNumbered( queue<block> &que, const block &target )
{
	block blocks[4];

	blocks[0] = block( target.row, target.col - 1 );
	blocks[1] = block( target.row, target.col + 1 );
	blocks[2] = block( target.row - 1, target.col );
	blocks[3] = block( target.row + 1, target.col );

	for ( int i = 0; i < 4; i++ )
	{
		if ( isValid( blocks[i] ) && getNum( blocks[i] ) > 0 )
		{
			que.push( blocks[i] );
		}
	}
}

void Solver::queueIntSur( queue<int> &que, const block &target )
{
	for ( int i = -1; i < 2; i++ )
	{
		for ( int j = -1; j < 2; j++ )
		{
			// if is a surrending unit
			if ( !( i == 0 && j == 0 ) )
			{
				int row = target.row + i;
				int col = target.col + j;
				// valid position
				if ( ( row >= 0 && row < game->numRows() ) && ( col >= 0 && game->numCols() ) )
				{
					que.push( getNum( block( row, col ) ) );
				}
			}
		}
	}
}
void Solver::showFeedback( const block &target )
{
	cout << "Target: ";
	target.print();
	//game->print();
	cout << endl;
	//cin.get();
}

void Solver::saveBoard( queue<int> &que )
{
	for ( int row = 0; row < game->numRows(); row++ )
	{
		for ( int col = 0; col < game->numCols(); col++ )
		{
			que.push( getNum( block( row, col ) ) );
		}
	}
}

void Solver::doubleClick( const block &target )
{
	game->revealSur( target.row, target.col );
}

bool Solver::checkChanged( bool changedSure, queue<int> &before, queue<int> &after )
{
	changedSure = false;
	while ( !before.empty() )
	{
		if ( before.front() != after.front() )
		{
			changedSure = true;
			break;
		}
		before.pop();
		after.pop();
	}	return changedSure;
}

bool Solver::isValid( block &target )
{
	if ( target.row > 0
		 && target.col > 0
		 && target.row < game->numRows() - 1
		 && target.col < game->numCols() - 1 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

double Solver::checkPercent()
{
	int flags = game->minesMarked();
	int total = game->totalMines();
	double percentRevealed = (double)flags / (double)total;
	return percentRevealed;
}

double Solver::printWithNoGuess( double noGuess )
{
	if ( !guessed )
	{
		cout << endl;
		cout << endl;
		noGuess = checkPercent();
		cout << "Before: " << noGuess << endl;
		guessed = true;
		game->print();
	}
	return noGuess;
}

//void Solver::queueSur2( const block &target )
//{
//	for ( int i = -1; i < 2; i++ )
//	{
//		for ( int j = -1; j < 2; j++ )
//		{
//			// if is a surrending unit
//			if ( !( i == 0 && j == 0 ) )
//			{
//				int row = target.row + i;
//				int col = target.col + j;
//				// valid position
//				if ( ( row >= 0 && row < game->numRows() ) && ( col >= 0 && game->numCols() ) )
//				{
//					now2.push( block( row, col ) );
//				}
//			}
//		}
//	}
//}