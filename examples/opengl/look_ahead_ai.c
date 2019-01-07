//
// Created by Paul on 2/21/2018.
//
#include <GL/glut.h>
#include "snake.h"
#include <stdio.h>
const char title[] = "Snake";
const int windowWidth = 512;
const int windowHeight = 512;
const int refreshMillis = 40;
const int screenOffsetX = 300;
const int screenOffsetY = 100;
float temp = 0;
void initGL(){
    glClearColor(0.0,0.0,0.0,1.0); //black background
}
int partAt(int x, int y){
    cur = head;
    while(1){
        if(cur->x == x && cur->y == y)  return 1;
        if(!cur->next) return 0;
        cur = cur->next;
    }
}
int getDistance(int x, int y, int d){ // starting coordinates x,y, and distance d 
    int space = 0;
    while(x < WIDTH && x > 0 && y < HEIGHT && y > 0 && !partAt(x,y)){
        ++space;
        switch(d){
            case 0:
                ++x;
                break;
            case 1: 
                ++y;
                break;
            case 2:
                --x;
                break;
            case 3:
                ++y;
                break;
        }
    }
    return space; 
}
void processAI(){
    //check to see which directions are valid
    int invalid[4];
    invalid[0]= partAt(head->x+1,head->y);  //right
    invalid[1]= partAt(head->x,head->y+1);  //up
    invalid[2]= partAt(head->x-1, head->y); //left
    invalid[3]= partAt(head->x, head->y-1); //down
    
    //start by setting the direction to any valid one
    int i=0;
    while(invalid[direction]) {
        direction = (direction+1)%4;
        if(++i == 3) break;
    }
    if(head->x != mouse.x || head->y != mouse.y){ //do nothing if you just ate
        if(head->x < mouse.x){
            if(invalid[0]){
                // if there is more space above than below
                if(getDistance(head->x,head->y+1,1)>getDistance(head->x,head->y-1,3)){
                    printf("Decided there was more space above than below\n");
                    direction = 1;
                }
                else{
                    printf("Decided there was more space below than above\n");
                    direction = 3;
                }
            }
            else{
                direction = 0; //right
            }
        }
        else if (head->x > mouse.x){
            if(invalid[2]){
                // if there is more space above than below
                if(getDistance(head->x,head->y+1,1)>getDistance(head->x,head->y-1,3)){
                    printf("Decided there was more space above than below\n");
                    direction = 1;
                }
                else{
                    printf("Decided there was more space below than above\n");
                    direction = 3;
                }
            }
            else{
                direction = 2; //left 
            }
        }
        else if (!invalid[1] && head->y < mouse.y){
            if(invalid[1]){
                // if there is more space right than left 
                if (getDistance(head->x+1,head->y,0)>getDistance(head->x-1,head->y,2)){
                    printf("Decided there was more space to the right than left\n");
                    direction = 0;
                }
                else{
                    printf("Decided there was more space to the left than right\n");
                    direction = 2;
                }
            }
            else{ 
                direction = 1; //up
            }
        }
        else {
            if (invalid[3]) {
                // if there is more space right than left 
                if (getDistance(head->x+1,head->y,0)>getDistance(head->x-1,head->y,2)){
                    printf("Decided there was more space to the right than left\n");
                    direction = 0;
                }
                else{
                    printf("Decided there was more space to the left than right\n");
                    direction = 2;
                }
            }                
            else{
                direction = 3; //down 
            }
        }
    }
}
void display(){
    processAI();
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
    //glutSpecialFunc(specialKeys);
    glutTimerFunc(0, Timer, 0);
    initGL();
    glutMainLoop();
    return 0;
}
