
    Square** minefield;
    int rows;
    int cols;
    int mines;
    int numFlagged;
    // statistics:
    int correctFlagged;
    int numRevealed;

    bool firstRevealed;
    bool checkSolvable;
    void createNewField(); // for constructor or remaking field
    void setNumbers();
    bool isSolvable() const; // checks certain cases that makes it unsolvable
    void printStats() const;
    void printEndStats() const;
    void printSol( int row, int col ) const;
    void gameOver( int row, int col, bool win ); // will be private