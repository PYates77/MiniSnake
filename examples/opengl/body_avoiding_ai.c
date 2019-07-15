//
// Created by Paul on 2/21/2018.
//
#include <GL/glut.h>
#include "snake.h"
#include <stdio.h>
const char title[] = "Snake";
const int windowWidth = 512;
const int windowHeight = 512;
const int refreshMillis = 50;
const int screenOffsetX = 300;
const int screenOffsetY = 100;
SnakeGame *game;
float temp = 0;

void initGL()
{
    glClearColor(0.0,0.0,0.0,1.0); //black background
}
int partAt(int x, int y)
{
    SnakePart *cur = game->head;
    while(1){
        if(cur->x == x && cur->y == y)  return 1;
        if(!cur->next) return 0;
        cur = cur->next;
    }
}
void processAI()
{
    //check to see which directions are valid
    int invalid[4];
    invalid[0]= partAt(game->head->x+1,game->head->y);  //right
    invalid[1]= partAt(game->head->x,game->head->y+1);  //up
    invalid[2]= partAt(game->head->x-1, game->head->y); //left
    invalid[3]= partAt(game->head->x, game->head->y-1); //down
    
    //start by setting the direction to any valid one
    int i=0;
    while(invalid[game->direction]) {
        game->direction = (game->direction+1)%4;
        if(++i == 3) break;
    }
    if(game->head->x != game->mouse.x || game->head->y != game->mouse.y){ //do nothing if you just ate
        if(!invalid[0] && game->head->x < game->mouse.x){
            game->direction = 0; //right
        }
        else if (!invalid[2] && game->head->x > game->mouse.x){
            game->direction = 2; //left 
        }
        else if (!invalid[1] && game->head->y < game->mouse.y){
            game->direction = 1; //up
        }
        else if (!invalid[3]) {
            game->direction = 3; //down 
        }
    }
}

void display()
{
    processAI();
    snake_tick(game);
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
void Timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(refreshMillis, Timer,0);
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    game = snake_init(windowHeight/16, windowWidth/16);

    glutInit(&argc, argv);
    //glutInitWindowSize(windowWidth, windowHeight);
    //glutInitWindowPosition(screenOffsetX,screenOffsetY);
    glutCreateWindow(title);
    glutDisplayFunc(display);
    glutTimerFunc(0, Timer, 0);
    initGL();
    glutMainLoop();
    return 0;
}
