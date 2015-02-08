// Minesweeper
// written by Russell Miller

#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include "square.h"

/*
    Minesweeper Class

    Minesweeper( rows, cols, mines, seed ) constructor
        Generatates a minefield with a minimum size of 9x9 and a maximum mine:size ratio of 90%. Impossible-to-solve checking is disabled at a mine:size ratio of 25%.
        rows - number of desired rows in minefield
        cols - number of desired columns in minefield
        mines - number of desired mines in minefield
        seed - seed for random number generator
    */

    /***************PUBLIC OPERATIONS***************/

    /*
    int numRows() const
        returns the number of rows in the minefield.

    int numCols() const
        returns the number of columns in the minefield.

    int totalMines() const
        returns the total number of mines in the minefield.

    int minesMarked() const
        returns the total number of flagged Squares in the minefield.
        The return value does not distinguish between correctly and incorrectly flagged Squares.
    */

    /*
    int revealSquare( row, col )
        Reveals the specified Square.
        It will not reveal a flagged Square.  If a mine is revealed or all non-mines are revealed, the program will be ended, and the solution will be printed.  If the revealed Square is a 0, all surrounding Squares will automatically be revealed.
        If impossible-to-solve checking is enabled, new minefields may be generated, and the first revealed square will be a 0.
        returns the number of the revealed Square
        returns -1 if the Square is flagged

    int isRevealed( row, col ) const
        returns the number of the revealed Square
        returns -1 if the Square is not revealed

    void flagSquare( row, col )
        Toggles the flag of the specified Square.
        It will not flag a revealed Square.

    bool isFlagged( row, col ) const
        returns whether the specified Square is flagged.

    void questionSquare( row, col )
        Toggles the QuestionMark flag of the specified Square.
        This flag is ignored by all other functions.

    bool isQuestion( row, col ) const
        returns whether the specified Square is flagged with a QuestionMark.
    */

    /*
    int surFlags( row, col ) const
        returns the number of Squares surrounding the specified Square that are flagged.

    int surEmpty( row, col ) const
        returns the number of Squares surrounding the specified Square that are unflagged and unrevealed.

    int revealSur( row, col )
        Reveals all the squares surrounding the specified Square if the specified Square is unflagged, revealed, and the number of flags surrounding the specified Square is equal to the number of the specified Square.
        returns 1 if any of the conditions above were not met.
        returns 0 if successful.
    */

    /*
    void print() const
        Prints the minefield with some statistics.
        F - flag
        # - revealed number
        ? - question mark flag
        . - unrevealed

    void endGame()
        Ends the program and prints the solution with some statistics.
        (* - revealed mine if you lost by revealing a mine)
        F - flagged mine
        W - flagged number
        X - unflagged mine
        # - revealed number
        ? - question mark flag on unrevealed number
        . - unrevealed number
*/


class Minesweeper
{
  private:
    #include "minesweeper_hPrivate.h"
    
  public:
    Minesweeper( int rows, int cols, int mines, int seed );
    int numRows() const;
    int numCols() const;
    int totalMines() const;
    int minesMarked() const;

    // returns number of Square
    int revealSquare( int row, int col );
    int isRevealed( int row, int col ) const;

    void flagSquare( int row, int col );
    bool isFlagged( int row, int col ) const;
    void questionSquare( int row, int col );
    bool isQuestion( int row, int col ) const;

    int surFlags( int row, int col ) const;
    int surEmpty( int row, int col ) const;
    int revealSur( int row, int col );

    void print() const;
    void endGame();
}; // class Minesweeper

#endif