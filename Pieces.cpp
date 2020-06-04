#include <iostream>
#include <time.h>
#include <unistd.h>
#include <GL/glut.h>
#include "Pieces.h"
#include "Board.h"

Pieces::Pieces(int x, int y) {
    new_piece(x,y);
}

void Pieces::new_piece(int x, int y){
    rotation = rand()%4+1;
    x_origin = x;
    y_origin = y;
    srand(time(NULL));

    for(int i = 0; i < 4; i++){
        colors[i] = rand()%5+1;
    }

    type = rand()%5+1;
    
    switch(type){
        case 1:
            boxLocations[0][0] = x-2+2*rotationX[rotation%4];
            boxLocations[0][1] = y+2*rotationY[rotation%4];
            boxLocations[1][0] = x-1+1*rotationX[rotation%4];
            boxLocations[1][1] = y+1*rotationY[rotation%4];
            boxLocations[2][0] = x;
            boxLocations[2][1] = y;
            boxLocations[3][0] = x+1-1*rotationX[rotation%4];
            boxLocations[3][1] = y+1*rotationY[rotation%4];
            break;
        case 2:
            boxLocations[0][0] = x-1+2*rotationX_S[rotation%4];
            boxLocations[0][1] = y-1+2*rotationY_S[rotation%4];
            boxLocations[1][0] = x+1*rotationX_S2[rotation%4];
            boxLocations[1][1] = y-1+1*rotationY_S2[rotation%4];
            boxLocations[2][0] = x;
            boxLocations[2][1] = y;
            boxLocations[3][0] = x+1-1*rotationX_S3[rotation%4];
            boxLocations[3][1] = y+1*rotationY_S3[rotation%4];
            break;
        case 3:
            boxLocations[0][0] = x+1-2*rotationX_S[(rotation+3)%4];
            boxLocations[0][1] = y-1+2*rotationY_S[(rotation+1)%4];
            boxLocations[1][0] = x+1*rotationX_S2[rotation%4];
            boxLocations[1][1] = y-1+1*rotationY_S2[rotation%4];
            boxLocations[2][0] = x;
            boxLocations[2][1] = y;
            boxLocations[3][0] = x-1+1*rotationX_S3[rotation%4];
            boxLocations[3][1] = y-1*rotationY_S3[rotation%4];
            break;
        case 4:
            boxLocations[0][0] = x-1+2*rotationX_L[rotation%4];
            boxLocations[0][1] = y-1+2*rotationY_L[rotation%4];
            boxLocations[1][0] = x-1+1*rotationX_L2[rotation%4];
            boxLocations[1][1] = y-1*rotationY_L2[rotation%4];
            boxLocations[2][0] = x;
            boxLocations[2][1] = y;
            boxLocations[3][0] = x+1-1*rotationX_S3[rotation%4];
            boxLocations[3][1] = y+1*rotationY_S3[rotation%4];       
            break;
        case 5:
            boxLocations[0][0] = x+1-2*rotationX_S[(rotation+3)%4];
            boxLocations[0][1] = y-1+2*rotationY_S[(rotation+1)%4];
            boxLocations[1][0] = x+1-1*rotationX_S3[rotation%4];
            boxLocations[1][1] = y+1*rotationY_S3[rotation%4];
            boxLocations[2][0] = x;
            boxLocations[2][1] = y;
            boxLocations[3][0] = x-1+1*rotationX_S3[rotation%4];
            boxLocations[3][1] = y-1*rotationY_S3[rotation%4];      
            break;
    }
}

void Pieces::drawPiece(Board* b){
    for(int rows = 0; rows < 4; rows++){
        for(int cols = 0; cols < 1; cols++){
            b->displayBox(boxLocations[rows][cols], boxLocations[rows][cols+1], colors[rows]);
        }
    }
}

bool Pieces::setPosition(int x, int y, Board* b){
    x_origin = x;
    y_origin = y;
    int testLocation[4][2] = {0};
    switch(type){
        case 1:
            testLocation[0][0] = x-2+2*rotationX[rotation%4];
            testLocation[0][1] = y+2*rotationY[rotation%4];
            testLocation[1][0] = x-1+1*rotationX[rotation%4];
            testLocation[1][1] = y+1*rotationY[rotation%4];
            testLocation[2][0] = x;
            testLocation[2][1] = y;
            testLocation[3][0] = x+1-1*rotationX[rotation%4];
            testLocation[3][1] = y-1*rotationY[rotation%4];
            for(int box = 0; box<4; box++){
                while(testLocation[box][0]<0){ //prevents movement and rotation out of the left border
                    for(int i = 0; i < 4; i++){
                        testLocation[i][0] += 1;
                    }
                }
                while(testLocation[box][0] > b->numCols-1){ //prevents movement and rotation out of the right border
                    for(int i = 0; i < 4; i++){
                        testLocation[i][0] -= 1;
                    }
                }
                
            }
            
            if(collisionDetection(testLocation, b)){
                b->updateBoard(boxLocations, colors);
                return false;
            }

            for(int box = 0; box < 4; box++){
                for(int coord = 0; coord<2; coord++){
                    boxLocations[box][coord] = testLocation[box][coord];
                }
            }
            break;
        case 2:

            testLocation[0][0] = x-1+2*rotationX_S[rotation%4];
            testLocation[0][1] = y-1+2*rotationY_S[rotation%4];
            testLocation[1][0] = x+1*rotationX_S2[rotation%4];
            testLocation[1][1] = y-1+1*rotationY_S2[rotation%4];
            testLocation[2][0] = x;
            testLocation[2][1] = y;
            testLocation[3][0] = x+1-1*rotationX_S3[rotation%4];
            testLocation[3][1] = y+1*rotationY_S3[rotation%4];
            for(int box = 0; box<4; box++){
                while(testLocation[box][0]<0){
                    for(int i = 0; i < 4; i++){
                        testLocation[i][0] += 1;
                    }
                }
                while(testLocation[box][0] > b->numCols-1){
                    for(int i = 0; i < 4; i++){
                        testLocation[i][0] -= 1;
                    }
                }
                
            }
            
            if(collisionDetection(testLocation, b)){
                b->updateBoard(boxLocations, colors);
                return false;
            }

            for(int box = 0; box < 4; box++){
                for(int coord = 0; coord<2; coord++){
                    boxLocations[box][coord] = testLocation[box][coord];
                }
            }
            break;
        case 3:
            testLocation[0][0] = x+1-2*rotationX_S[(rotation+3)%4];
            testLocation[0][1] = y-1+2*rotationY_S[(rotation+1)%4];
            testLocation[1][0] = x+1*rotationX_S2[rotation%4];
            testLocation[1][1] = y-1+1*rotationY_S2[rotation%4];
            testLocation[2][0] = x;
            testLocation[2][1] = y;
            testLocation[3][0] = x-1+1*rotationX_S3[rotation%4];
            testLocation[3][1] = y-1*rotationY_S3[rotation%4];
            for(int box = 0; box<4; box++){
                while(testLocation[box][0]<0){
                    for(int i = 0; i < 4; i++){
                        testLocation[i][0] += 1;
                    }
                }
                while(testLocation[box][0] > b->numCols-1){
                    for(int i = 0; i < 4; i++){
                        testLocation[i][0] -= 1;
                    }
                }
                
            }
            
            if(collisionDetection(testLocation, b)){
                b->updateBoard(boxLocations, colors);
                return false;
            }

            for(int box = 0; box < 4; box++){
                for(int coord = 0; coord<2; coord++){
                    boxLocations[box][coord] = testLocation[box][coord];
                }
            }
            break;
        case 4:
            testLocation[0][0] = x-1+2*rotationX_L[rotation%4];
            testLocation[0][1] = y-1+2*rotationY_L[rotation%4];
            testLocation[1][0] = x-1+1*rotationX_L2[rotation%4];
            testLocation[1][1] = y-1*rotationY_L2[rotation%4];
            testLocation[2][0] = x;
            testLocation[2][1] = y;
            testLocation[3][0] = x+1-1*rotationX_S3[rotation%4];
            testLocation[3][1] = y+1*rotationY_S3[rotation%4];
            for(int box = 0; box<4; box++){
                while(testLocation[box][0]<0){
                    for(int i = 0; i < 4; i++){
                        testLocation[i][0] += 1;
                    }
                }
                while(testLocation[box][0] > b->numCols-1){
                    for(int i = 0; i < 4; i++){
                        testLocation[i][0] -= 1;
                    }
                }
                
            }
            
            if(collisionDetection(testLocation, b)){
                b->updateBoard(boxLocations, colors);
                return false;
            }

            for(int box = 0; box < 4; box++){
                for(int coord = 0; coord<2; coord++){
                    boxLocations[box][coord] = testLocation[box][coord];
                }
            }     
            break;
        case 5:
            testLocation[0][0] = x+1-2*rotationX_S[(rotation+3)%4];
            testLocation[0][1] = y-1+2*rotationY_S[(rotation+1)%4];
            testLocation[1][0] = x+1-1*rotationX_S3[rotation%4];
            testLocation[1][1] = y+1*rotationY_S3[rotation%4];
            testLocation[2][0] = x;
            testLocation[2][1] = y;
            testLocation[3][0] = x-1+1*rotationX_S3[rotation%4];
            testLocation[3][1] = y-1*rotationY_S3[rotation%4];
            for(int box = 0; box<4; box++){
                while(testLocation[box][0]<0){
                    for(int i = 0; i < 4; i++){
                        testLocation[i][0] += 1;
                    }
                }
                while(testLocation[box][0] > b->numCols-1){
                    for(int i = 0; i < 4; i++){
                        testLocation[i][0] -= 1;
                    }
                }
                
            }

            if(collisionDetection(testLocation, b)){
                b->updateBoard(boxLocations, colors);
                return false;
            }

            for(int box = 0; box < 4; box++){
                for(int coord = 0; coord<2; coord++){
                    boxLocations[box][coord] = testLocation[box][coord];
                }
            }
            
            break;
    }
    return true;
}

bool Pieces::collisionDetection(int coordinates[4][2], Board* b){
    for(int box = 0; box < 4; box++){
        if(coordinates[box][0] < b->numCols){
            if(b->stackHeights[coordinates[box][0]] > coordinates[box][1]){
                return true;
            }
        }
    }
    return false;
}

void Pieces::rotate(){
    rotation++;
    if(rotation > 4){
        rotation = 0;
    }
}


int* Pieces::getColors(){
    return colors;
}