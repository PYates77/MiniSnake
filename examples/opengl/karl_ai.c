//
// Created by Paul on 2/21/2018.
//
#include <GL/glut.h>
#include "../../snake.h"
#include <stdio.h>
#include <string.h>
const char title[] = "Snake";
#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512
const unsigned int windowWidth = WINDOW_WIDTH;
const unsigned int  windowHeight = WINDOW_HEIGHT;
const int refreshMillis = 50;
const int screenOffsetX = 300;
const int screenOffsetY = 100;
float temp = 0;
SnakeGame * game;
int maxDepth = 100;
int checkedCoordinates[WINDOW_WIDTH * WINDOW_HEIGHT];

void initGL(){
    glClearColor(0.0,0.0,0.0,1.0); //black background
}
int partAt(int x, int y){
    SnakePart * cur = game->head;
    while(1) {
        if(cur->x == x && cur->y == y)  return 1;
        if(!cur->next) return 0;
        cur = cur->next;
    }
}

//
void copySnake(SnakePart * headIn, SnakePart ** headOut, SnakePart ** tailOut)
{
    *headOut = malloc(sizeof(SnakePart));
    memcpy(*headOut, headIn, sizeof(SnakePart));
    SnakePart cur = **headOut;
    while(cur.next != NULL){
        SnakePart temp = *cur.next;
        cur.next = malloc(sizeof(SnakePart));
        *cur.next = temp;
        cur = *cur.next;
    }
    *tailOut = &cur; 
}

//returns either an int corresponding to a SnakeDirection which is the best direction
//or -1 if no direction is valid
int playRecursiveSnake(SnakeGame recursiveGame, int depth) 
{
    if(depth == maxDepth){
        return -1;
    }
    //prevent recursive function from checking the same coordinate more than once
    int checkedIndex = windowWidth * recursiveGame.head->y + recursiveGame.head->x;
    if(checkedCoordinates[checkedIndex]){
        return -1;
    }
    checkedCoordinates[checkedIndex] = 1;
    
    printf("Recursive snake at depth %d, head at (%d x %d)\n", depth, recursiveGame.head->x, recursiveGame.head->y);
    SnakeGame games[4];
    int i;
    for(i=0; i<4; ++i){
        memcpy(&games[i], &recursiveGame, sizeof(SnakeGame));
        copySnake(recursiveGame.head, &games[i].head, &games[i].tail);
    }

    games[DIRECTION_POS_X].direction = DIRECTION_POS_X;
    games[DIRECTION_POS_Y].direction = DIRECTION_POS_Y;
    games[DIRECTION_NEG_X].direction = DIRECTION_POS_X;
    games[DIRECTION_NEG_Y].direction = DIRECTION_NEG_Y;
        
    int numAliveSnakes = 0;
    int aliveSnake = 0;
    for(i=0; i<4; ++i){
        snake(&games[i]);
        if(games[i].flag && !games[i].gameover){
            //mouse eaten! return this direction!!!
            printf("\n\n\n\n\n\nMOUSE EATEN PATH FOUND\n\n\n\n\n");
            return i;
        }
        if(!games[i].gameover){
            int d = playRecursiveSnake(games[i], depth+1);
            if(d != -1){
                //if this is a path which results in eating a mouse, return this direction
                return d;
            }
        }
    }
    //no mouse eaten direction found, return failure
    return -1;
}

void processAI(){
    int i;
    for(i=0; i<windowWidth*windowHeight; ++i){
        checkedCoordinates[i] = 0;
    }
    int d = playRecursiveSnake(*game, 0);
    if(d == -1){
        printf("No path found!!! Defaulting to direction 0\n");
        d = 0;
    }
    game->direction = d;
#if 0 
    //check to see which directions are valid
    int invalid[4];
    SnakePart * head = game->head;
    invalid[0]= partAt(head->x+1,head->y);  //right
    invalid[1]= partAt(head->x,head->y+1);  //up
    invalid[2]= partAt(head->x-1, head->y); //left
    invalid[3]= partAt(head->x, head->y-1); //down

    SnakePart mouse = game->mouse;
    
    //start by setting the direction to any valid one
    int i=0;
    while(invalid[game->direction]) {
        game->direction = (game->direction+1)%4;
        if(++i == 3) break;
    }
    if(head->x != mouse.x || head->y != mouse.y){ //do nothing if you just ate
        if(!invalid[0] && head->x < mouse.x){
            game->direction = 0; //right
        }
        else if (!invalid[2] && head->x > mouse.x){
            game->direction = 2; //left 
        }
        else if (!invalid[1] && head->y < mouse.y){
            game->direction = 1; //up
        }
        else if (!invalid[3]) {
            game->direction = 3; //down 
        }
    }
#endif 
}

void display()
{
    processAI();
    snake(game);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    SnakePart * cur = game->head;
    float offsetX = 1.0f/(game->width/2);
    float offsetY = 1.0f/(game->height/2);
    glBegin(GL_QUADS);
    SnakePart mouse = game->mouse;
    int x_adjust = mouse.x-game->width/2;
    int y_adjust = mouse.y-game->height/2;
    float x = (float)(x_adjust)/(game->width/2);
    float y = (float)(y_adjust)/(game->height/2);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(x,y);
    glVertex2f(x,y+offsetY);
    glVertex2f(x+offsetX,y+offsetY);
    glVertex2f(x+offsetX,y);
    glEnd();
    while(1){
        int x_adjust = cur->x-game->width/2;
        int y_adjust = cur->y-game->height/2;
        float x = (float)(x_adjust)/(float)(game->width/2);
        float y = (float)(y_adjust)/(float)(game->height/2);
        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(x, y);
        glVertex2f(x, y+offsetY);
        glVertex2f(x+offsetX, y+offsetY);
        glVertex2f(x+offsetX, y);
        glEnd();
        if(cur->next) cur = cur->next;
        else break;
    }

    glFlush();


}
void Timer(int value){
    glutPostRedisplay();
    glutTimerFunc(refreshMillis, Timer,0);
}
int main(int argc, char** argv){
    srand(time(NULL));
    game = snake_init(windowHeight/16, windowWidth/16);

    glutInit(&argc, argv);
    //glutInitWindowSize(windowWidth, windowHeight);
    //glutInitWindowPosition(screenOffsetX,screenOffsetY);
    glutCreateWindow(title);
    glutDisplayFunc(display);
    //glutSpecialFunc(specialKeys);
    glutTimerFunc(0, Timer, 0);
    initGL();
    glutMainLoop();
    return 0;
}
