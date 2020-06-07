#include <iostream>
#include <GL/glut.h>
#include "Board.h"

Board::Board(){
    for(int rows = 0; rows<numRows; rows++){
        for(int cols = 0; cols<numCols; cols++){
            stacks[cols][rows] = 0;
        }
    }
}

void Board::displayBox(int x, int y, int color){
    //sets color of the square
	switch(color){
	case 1: //Red
		glColor3f (1.0, 0.0, 0.0);
		break;
	case 2: //Yellow
		glColor3f (1.0, 1.0, 0.0);
		break;
	case 3: //Green
		glColor3f (0.0, 1.0, 0.0);
		break;
	case 4: //Purple
		glColor3f (0.8, 0.0, 1.0);
		break;
	case 5://Orange
		glColor3f (1.0, 0.5, 0.20);
		break;
    case 6:
        glColor3f(0.75, 0.75, 0.75);
        break;
	}

    if(y<numRows){ //prevents boxes showing from the top of the grid
        //convert matrix index to window space coordinate
        x=(xLocation)+x*boxSize;
        y=(yLocation)-(numRows-y-1)*boxSize;

        //draw a box at the given x and y
        glBegin(GL_POLYGON);
            glVertex2f(x, y-0.5);
            glVertex2f(x+boxSize-0.5, y-0.5);
            glVertex2f(x+boxSize-0.5, y-boxSize);
            glVertex2f(x, y-boxSize);
        glEnd();
    }
}

void Board::drawBoard(){
    glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);
    //draw the grid
	for(int i = 0; i < numCols; i++){
		for(int j = 0; j < numRows; j++){
			glBegin(GL_LINE_LOOP);
				glVertex2f(xLocation+boxSize*i, yLocation-boxSize*j);
				glVertex2f(xLocation+boxSize*(i+1), yLocation-boxSize*j);
				glVertex2f(xLocation+boxSize*(i+1), yLocation-boxSize*(j+1));
				glVertex2f(xLocation+boxSize*i, yLocation-boxSize*(j+1));
			glEnd();
		}
	}

    //draw the pieces that have already landed
    for(int cols = 0; cols<numCols; cols++){
        for(int rows = 0; rows<numRows; rows++){
            if(stacks[cols][rows]!=0){
                displayBox(cols, rows, stacks[cols][rows]);
            }
        }
    }
}

void Board::updateBoard(int locations[4][2], int colors[4]){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 1; j++){
            //Update the Logic Matrix
            stacks[locations[i][j]][locations[i][j+1]] = colors[i];
             //update the height of each column 
            stackHeights[locations[i][j]] = locations[i][j+1]+1;
        }
    }
    //checks for game over, line completion, fruit matching
    std::cout<<"Before State Check: "<<std::endl;
    printStacks();
    checkGameState();
    std::cout<<"After State Check and Adjust: "<<std::endl;
    printStacks();
}


/*
 * If a fruit is matched, this function brings all the fruit above it down
 */
void Board::dropFruit(int x, int y){
	int i = y;
	while(stacks[x][i+1] != 0 && i < numRows-1){
		stacks[x][i] = stacks[x][i+1];
		stacks[x][i+1] = 0;
		i++;
	}
}

/*
 * when deletion is vertical (3 fruits stacked vertically)
 */
void Board::dropFruitVertical(int x, int y){
	int i = y;
	while(stacks[x][i+3] != 0 && i < numRows-3){
		stacks[x][i] = stacks[x][i+3];
		stacks[x][i+3] = 0;
		i++;
	}
}


void Board::checkGameState(){
    //Game Over Check
    for(int cols = 0; cols < numCols; cols++){
        if(stacks[cols][numRows-1]!=0){
            game_over = true;
            break;
        }
    }

    //Sets all the landed blocks grey
    if(game_over){
        for(int cols = 0; cols < numCols; cols++){
            for(int rows = 0; rows < numRows; rows++){
                if(stacks[cols][rows]!=0){
                    stacks[cols][rows]=6;
                }
            }
        }
        return;
    }
    //Line Deletion
    bool row_complete = true;
    int complete_rows[numRows] = {0};
    for(int rows = 0; rows < numRows; rows++){
        for(int cols = 0; cols < numCols; cols++){
            if(stacks[cols][rows] == 0){
                row_complete = false;
            }
            stackHeights[cols] = 0;
        }

        if(row_complete){
            for(int cols = 0; cols < numCols; cols++){
                stacks[cols][rows] = 0;
                complete_rows[rows] = 1;
            }
        }
    row_complete = true;
    }

    int consec_rows = 0;
    int first_deleted_row = 0; 
    for(int rows = 0; rows<numRows-1; rows++){ 
        if(complete_rows[rows]==1){
            if(consec_rows == 0){
                first_deleted_row = rows;
            }
            if(complete_rows[rows+1] == 1){ //finds number of consecutive deleted rows
                consec_rows++;
            }else{
                for(int copy = first_deleted_row; copy < numRows-consec_rows-1; copy++){
                    for(int cols = 0; cols<numCols; cols++){
                        stacks[cols][copy] = stacks[cols][copy+consec_rows+1];
                    }

                    //NOTE: Potential Memory Bug, this line modifies nothing out of its scope
                    //But there is a bug that is solved by having this line in. WIP. 
                    for(int rows = 0; rows<numRows; rows++){
                    }
                    //shift the completed rows array to match the logic matrix
                    complete_rows[copy-consec_rows-1] = complete_rows[copy];
                }
                consec_rows = 0;
                first_deleted_row = 0;
            }
        }
    }

	//same colour fruit deletion
	int colour,
		left,
		right,
		top,
		bottom,
		top_left,
		top_right,
		bottom_left,
		bottom_right;
	/*
	 * checks the inner perimeter of the matrix using a 3x3 neighbourhood to check for matches
	 */
    //[col][row]
    check:
        for(int i = 1; i < numCols-1; i++){
            for(int j = 1; j< numRows-1; j++){
                colour = stacks[i][j];
                left = stacks[i-1][j];
                right = stacks[i+1][j];
                top = stacks[i][j+1];
                bottom = stacks[i][j-1];
                top_left = stacks[i-1][j+1];
                top_right = stacks[i+1][j+1];
                bottom_left = stacks[i-1][j-1];
                bottom_right = stacks[i+1][j-1];


                if(bottom_left == colour && top_right == colour && colour != 0){
                    //Delete the fruit
                    stacks[i-1][j-1] = 0;
                    stacks[i][j] = 0;
                    stacks[i+1][j+1] = 0;
                    //Bring Fruit Down
                    dropFruit(i-1 ,j-1);
                    dropFruit(i ,j);
                    dropFruit(i+1 ,j+1);
                    goto check; //to double check for cascading fruit matching
                }

                if(bottom_right == colour && top_left == colour && colour != 0){
                    //Delete the Fruit
                    stacks[i+1][j-1] = 0;
                    stacks[i][j] = 0;
                    stacks[i-1][j+1] = 0;
                    //Bring Fruit Down
                    dropFruit(i+1 ,j-1);
                    dropFruit(i ,j);
                    dropFruit(i-1 ,j+1);
                    goto check; 
                }

                if(left == colour && right == colour && colour != 0){
                    //Delete the Fruit
                    stacks[i-1][j] = 0;
                    stacks[i][j] = 0;
                    stacks[i+1][j] = 0;
                    //Bring Fruit Down
                    dropFruit(i-1 ,j);
                    dropFruit(i ,j);
                    dropFruit(i+1 ,j);
                    goto check; 
                }

                if(top == colour && bottom == colour && colour != 0){
                    //Delete the Fruit
                    stacks[i][j+1] = 0;
                    stacks[i][j] = 0;
                    stacks[i][j-1] = 0;
                    //Bring Fruit Down
                    dropFruitVertical(i, j);
                    dropFruitVertical(i, j+1);
                    dropFruitVertical(i, j-1);
                    goto check; 
                }
            }
	    }

	// //Deletion on Borders (Bottom Row)
	for(int j = 1; j< numCols-1; j++){
		if(stacks[j][0] == stacks[j+1][0] && stacks[j-1][0] == stacks[j][0] && stacks[j][0]!=0){
			stacks[j][0] = 0;
			stacks[j-1][0] = 0;
			stacks[j+1][0] = 0;
			//Bring Fruit Down
			dropFruit(j ,0);
			dropFruit(j-1 ,0);
			dropFruit(j+1 ,0);
		}
	}
	//Deletion on Borders (Left and right Borders)
	for(int i = 0; i < numCols; i+=(numCols)-1){
		for(int j = 1; j < numRows; j++){
			if(stacks[i][j] == stacks[i][j+1] && stacks[i][j] == stacks[i][j-1] && stacks[i][j]!=0){
				stacks[i][j] = 0;
				stacks[i][j-1] = 0;
				stacks[i][j+1] = 0;
				dropFruitVertical(i, j);
				dropFruitVertical(i, j+1);
				dropFruitVertical(i, j-1);
			}
		}
	}


    //update stack heights
    for(int rows = 0; rows < numRows; rows++){
        for(int cols = 0; cols < numCols; cols++){
            if(stacks[cols][rows]!=0){
                stackHeights[cols] = rows+1;
            }
        }
    }

}

int Board::getNumCols(){
	return numCols;
}

int Board::getNumRows(){
	return numRows;
}

void Board::reset(){
    game_over = false;
    for(int cols = 0; cols < numCols; cols++){
        for(int rows = 0; rows < numRows; rows++){
            stacks[cols][rows] = 0;
        }
        stackHeights[cols] = 0;
    }
}

bool Board::getGameState(){
    return game_over;
}

/*
FOR DEBUGGING
*/
void Board::printStacks(){
    for(int rows = numRows-1; rows >= 0; rows--){
        for(int cols = 0; cols < numCols; cols++){
            std::cout<<stacks[cols][rows]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

void Board::printStackHeights(){
    for(int i = 0; i < numCols; i++){
        std::cout<<stackHeights[i]<<" ";
    }
    std::cout<<std::endl;
}