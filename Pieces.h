class Board;
class Pieces{
    public:
        int x_origin;
        int y_origin;
        

    Pieces(int x, int y);

    void new_piece(int x, int y);

    void displayBox(int x, int y, int color);

    void drawPiece(Board*);

    bool setPosition(int x, int y, Board* b);

    bool collisionDetection(int coordinates[4][2], Board* b);

    void rotate();

    int* getColors();


    private:
        int boxLocations[4][2] = {0};
        int type;
        int rotation;
        int rotationX[4] = {0, 1, 2, 1};
        int rotationY[4] = {0, -1, 0, 1};
        int rotationX_S[4] = {0, 1, 1, 0};
        int rotationX_S2[4] = {0, 1, 0, -1};
        int rotationX_S3[4] = {0, 1, 2, 1};
        int rotationY_S[4] = {0, 0, 1, 1};
        int rotationY_S2[4] = {0, 1, 2, 1};
        int rotationY_S3[4] = {0, 1, 0, -1};
        int rotationX_L[4] = {0, 1, 1, 0};
        int rotationY_L[4] = {0, 0, 1, 1};
        int rotationX_L2[4] = {0, 1, 2, 1}; 
        int rotationY_L2[4] = {0, 1, 0, -1};
        int colors[4] = {1};
};