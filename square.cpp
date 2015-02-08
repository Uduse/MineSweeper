// Minesweeper Square
// written by Russell Miller

#include <iostream>
#include "square.h"

using namespace std;

Square::Square()
  : mine(false), flagged(false), number(0), revealed(false)
{} // Square constructor


bool Square::isMine() const
{ return mine; }

void Square::setMine()
{ mine = true; }

bool Square::isFlagged() const
{ return flagged; }

void Square::setFlagged() // toggles
{ flagged = !flagged; }

bool Square::isQuestion() const
{ return flagged; }

void Square::setQuestion() // toggles
{ questionFlag = !questionFlag; }

int Square::getNumber() const
{ return number; }

void Square::setNumber( int num )
{ number = num; }

bool Square::isRevealed() const
{ return revealed; }

void Square::reveal()
{ revealed = true; }