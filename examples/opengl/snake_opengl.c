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
float temp = 0;
void initGL(){
    glClearColor(0.0,0.0,0.0,1.0); //black background
}
void specialKeys(int key, int mouseX, int mouseY){
    switch(key){
        case GLUT_KEY_RIGHT:
            direction = 0;
            break;
        case GLUT_KEY_UP:
            direction = 1;
            break;
        case GLUT_KEY_LEFT:
            direction = 2;
            break;
        case GLUT_KEY_DOWN:
            direction = 3;
            break;
    }
}
void display(){
    snake();
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    cur = head;
    float offsetX = 1.0f/(WIDTH/2);
    float offsetY = 1.0f/(HEIGHT/2);
    glBegin(GL_QUADS);
    float x = (float)(mouse.x-HEIGHT/2)/(HEIGHT/2);
    float y = (float)(mouse.y-HEIGHT/2)/(HEIGHT/2);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(x,y);
    glVertex2f(x,y+offsetY);
    glVertex2f(x+offsetX,y+offsetY);
    glVertex2f(x+offsetX,y);
    glEnd();
    while(1){
        float x = (float)(cur->x-HEIGHT/2)/(HEIGHT/2);
        float y = (float)(cur->y-HEIGHT/2)/(HEIGHT/2);
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
    head = (SnakePart*)malloc(sizeof(SnakePart));
    head->x = WIDTH>>1;
    head->y = HEIGHT>>1;
    head->next = NULL;
    head->prev = head;
    tail = head;
    mouse.x = rand()%WIDTH;
    mouse.y = rand()%HEIGHT;

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
