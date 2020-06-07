

class Board{
    friend class Pieces;
    public:
        int xLocation = 16;
        int yLocation = 340; //370
        int boxSize = 15; //17 is ideal


    Board();

    void drawBoard();

    void updateBoard(int locations[4][2], int colors[4]);

    int getBottom();

    int getNumCols();

    int getNumRows();

    void reset();

    bool getGameState();

    //Debugging
    void printStacks();
    void printStackHeights();

    private:
        bool game_over = false;
        const static int numCols = 10;
        const static int numRows = 20;
        int stacks[numCols][numRows];
        int stackHeights[numCols] = {0};
        void displayBox(int x, int y, int color);
        void checkGameState();
        void dropFruitVertical(int x, int y);
        void dropFruit(int x, int y);

    
};