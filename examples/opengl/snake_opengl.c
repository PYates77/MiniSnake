//
// Created by Paul on 2/21/2018.
//
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include <time.h>
#include "snake.h"
const char title[] = "Snake";
const int windowWidth = 512;
const int windowHeight = 512;
const int refreshMillis = 50;
const int screenOffsetX = 300;
const int screenOffsetY = 100;
SnakeGame * game;
float temp = 0;
void initGL(){
    glClearColor(0.0,0.0,0.0,1.0); //black background
}
void specialKeys(int key, int mouseX, int mouseY){
    switch(key){
        case GLUT_KEY_RIGHT:
            game->direction = 0;
            break;
        case GLUT_KEY_UP:
            game->direction = 1;
            break;
        case GLUT_KEY_LEFT:
            game->direction = 2;
            break;
        case GLUT_KEY_DOWN:
            game->direction = 3;
            break;
    }
}
void display()
{
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
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, Timer, 0);
    initGL();
    glutMainLoop();
    return 0;
}
