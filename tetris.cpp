#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>
#include <GL/glut.h>
#include "Board.h"
#include "Pieces.h"

int SCREEN_WIDTH = 350;
int SCREEN_HEIGHT = 700;
auto LAST_UPDATE = std::chrono::system_clock::now();
//X Position, Y Position, Number of Columns, Number of Rows, Size of boxes
Board board;
Pieces piece(5,19);
bool force_update = false;
bool success = true;
bool PAUSED = false;
bool crtl_pressed = false;

void drawGame(void){
    if(!PAUSED && !board.getGameState()){
        board.drawBoard();

        if(!success){
            piece.new_piece(5,19);
            success = true;
        }
        piece.drawPiece(&board);

        auto curr_time = std::chrono::system_clock::now();
        std::chrono::duration<double>time_since_update = curr_time - LAST_UPDATE;
        if(time_since_update.count() > 1 || force_update){
            success = piece.setPosition(piece.x_origin, piece.y_origin-1, &board);
            force_update = false;
            LAST_UPDATE = std::chrono::system_clock::now();
        }
        glFlush();
        glutSwapBuffers();
    }
    if(board.getGameState()){
        board.drawBoard();
        glFlush();
        glutSwapBuffers();
    }
}


void keypress_listener(int key, int x, int y){
    //CRTL Key for Camera Movement
    int mod_key = glutGetModifiers();
    if(mod_key == GLUT_ACTIVE_CTRL){
        crtl_pressed = true;
       
    }else if(crtl_pressed){
        crtl_pressed = false;
    }

	switch(key){
	case GLUT_KEY_LEFT:
        if(crtl_pressed){

        }else{
            piece.setPosition(piece.x_origin-1, piece.y_origin, &board);
        }

		break;
	case GLUT_KEY_RIGHT:
        if(crtl_pressed){
            
        }else{
            piece.setPosition(piece.x_origin+1, piece.y_origin, &board);
        }

        break;
	case GLUT_KEY_UP:
        piece.rotate();
        piece.setPosition(piece.x_origin, piece.y_origin, &board);
        break;
	case GLUT_KEY_DOWN:
		force_update = true;
        break;
	}

}

void keypress(unsigned char key, int x, int y){
	if(key == 'r'){ //restart game
        board.reset();
        piece.new_piece(5,19);
	}

	if(key =='p'){ //pause game and resume game
		PAUSED = !PAUSED;
	}
}
void init (void) {
	glClearColor (0, 0, 0, 0);
	glMatrixMode (GL_PROJECTION);
	gluOrtho2D(0.0, SCREEN_WIDTH/2, 0.0, SCREEN_HEIGHT/2);
}

int main(int argc, char** argv){
    srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("A Falling Fruits Game");

	init();
	glutDisplayFunc(drawGame);
    glutIdleFunc(drawGame);
    glutKeyboardFunc(keypress);
    glutSpecialFunc(keypress_listener);
	glutMainLoop();
	return 0;
}

