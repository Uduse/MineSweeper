// Minesweeper Square
// written by Russell Miller

#ifndef SQUARE_H
#define SQUARE_H

class Square
{
  private:
    bool mine;
    bool flagged;
    bool questionFlag;
    int number;
    bool revealed; // known
  public:
    Square();
    bool isMine() const;
    void setMine();

    bool isFlagged() const;
    void setFlagged(); // toggle
    
    bool isQuestion() const;
    void setQuestion(); // toggle
    
    int getNumber() const;
    void setNumber( int num ); // changeable, does this matter?
    
    bool isRevealed() const;
    void reveal();
}; // class Square


#endif