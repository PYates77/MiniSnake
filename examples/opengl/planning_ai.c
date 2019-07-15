//
// Created by Paul on 2/21/2018.
//
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif 

#include "snake.h"
#include <stdio.h>

#define DRAW_PLAN 0

const char title[] = "Snake";
const int windowWidth = 512;
const int windowHeight = 512;
const int refreshMillis = 50;
const int screenOffsetX = 300;
const int screenOffsetY = 100;
SnakeGame *game;
float temp = 0;

struct vec2 {
    int x;
    int y;
};

enum SnakeDirection plan[512*512];
struct vec2 plan_squares[512*512];
int bad_squares[512*512]; // squares deemed bad by the planning algorithm 
int plan_len = 0;
int plan_index = 0;

void initGL()
{
    glClearColor(0.0,0.0,0.0,1.0); //black background
}

/* Will there be a part here at some time step? */
int partAtLater(int x, int y, int t) 
{
    if (x < 0) {
        x = game->width - 1;
    }
    if (y < 0) {
        y = game->height - 1;
    }
    x = x % game->width;
    y = y % game->height;

    SnakePart *cur = game->tail;
    /* Skip the first t snake parts from the tail */
    int i;
    for(i=0; i<t; ++i) {
        if (!cur->prev) return 0;
        cur = cur->prev;
    }

    while(1) {
        if(cur->x == x && cur->y == y) return 1;
        if (!cur->prev) return 0;
        cur = cur->prev;
    }

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

int bad(int x, int y) {
    if (bad_squares[512*y+x]) {
       return 1;
    } else {
        return 0;
    }
}

int planned(int x, int y) {
    int i;
    if (x < 0) {
        x = game->width - 1;
    }
    if (y < 0) {
        y = game->height - 1;
    }
    x = x % game->width;
    y = y % game->height;

    // if plan_index > current snake length, we can ignore the first plan_index - snake_len planned squares, 
    // since the snake will already be past this point
    int snake_len = game->score + 1;
    int i0 = 0;
    if (plan_index > snake_len) {
        i0 = plan_index - snake_len;
    }

    for (i=i0; i<plan_len - plan_index; ++i){
        if(plan_squares[i].x == x && plan_squares[i].y == y) {
            return 1;
        }
    }
    return 0;
}

int body_avoiding_ai_planner(int x, int y, enum SnakeDirection cur_direction)
{
    int invalid[4];
    invalid[0] = partAtLater(x+1,y, plan_len) || bad(x+1,y) || planned(x+1, y);  //right
    invalid[1] = partAtLater(x,y+1, plan_len) || bad(x,y+1) || planned(x,y+1);  //up
    invalid[2] = partAtLater(x-1, y, plan_len) || bad(x-1, y) || planned(x-1, y); //left
    invalid[3] = partAtLater(x,y-1, plan_len) || bad(x,y-1) || planned(x,y-1); //down

    //printf("Planning at %d, %d, invalid[] = {%d, %d, %d, %d}\n", x,y,invalid[0], invalid[1], invalid[2], invalid[3]);

    // if all directions are invalid, this is not a valid plan
    if (invalid[0] && invalid[1] && invalid[2] && invalid[3]) {
        //printf("NO VALID PATHS! BACKING UP!\n");
        return 0;
    }

    //start by setting the direction to any valid one
    int i=0;
    while(invalid[cur_direction]) {
        cur_direction= (cur_direction+1)%4;
        if(++i == 3) break;
    }

    if(x != game->mouse.x || y != game->mouse.y){ 
        if(!invalid[0] && x < game->mouse.x) {
            cur_direction = 0; //right
        } else if (!invalid[2] && x > game->mouse.x) {
            cur_direction = 2; //left 
        } else if (!invalid[1] && y < game->mouse.y) {
            cur_direction = 1; //up
        } else if (!invalid[3]) {
            cur_direction = 3; //down 
        } 

        switch(cur_direction) {
            case 0:
                plan_squares[plan_len].x = x+1;
                plan_squares[plan_len].y = y;
                break;
            case 1:
                plan_squares[plan_len].x = x;
                plan_squares[plan_len].y = y+1;
                break;
            case 2:
                /* handle underflow */
                if (x-1 < 0) {
                    plan_squares[plan_len].x = game->width - 1;
                } else {
                    plan_squares[plan_len].x = x-1;
                }
                plan_squares[plan_len].y = y;
                break;
            case 3:
                /* handle underflow */
                if (y-1 < 0) {
                    plan_squares[plan_len].y = game->height - 1;
                } else {
                    plan_squares[plan_len].y = y-1;
                }
                plan_squares[plan_len].x = x;
                break;
        }

        /* handle overflow */
        plan_squares[plan_len].x = plan_squares[plan_len].x % game->width;
        plan_squares[plan_len].y = plan_squares[plan_len].y % game->height;

        plan[plan_len] = cur_direction;
        //printf("Plan head (%d, %d). Adding direction %d to plan[%d] (%d, %d)\n", x, y, plan[plan_len], plan_len, plan_squares[plan_len].x, plan_squares[plan_len].y);
        ++plan_len;
        
        int res = body_avoiding_ai_planner(plan_squares[plan_len-1].x, plan_squares[plan_len-1].y, cur_direction);

        if (res) {
            return 1;
        } else {
            // add the planned square to the set of bad squares
            //printf("adding (%d, %d) to bad squares\n",plan_squares[plan_len-1].x, plan_squares[plan_len-1].y);
            bad_squares[plan_squares[plan_len-1].y * 512 + plan_squares[plan_len-1].x] = 1;
            --plan_len;
            return body_avoiding_ai_planner(x, y, cur_direction);
        } 

    } else {
        return 1; // return success if we have reached mouse position
    }
}

void make_plan() 
{
    plan_len = 0;
    for(int i=0; i<512*512; ++i){
        bad_squares[i] = 0;
    }
    
    int ret = body_avoiding_ai_planner(game->head->x, game->head->y, game->direction);
    if (ret == 0) {
        //printf("\nNo valid plan found :(\n");
    } 
}

void processAI()
{
    if((plan_index >= plan_len) || (game->head->x == game->mouse.x && game->head->y == game->mouse.y)){ 
        //plan has ended, make new plan
        //printf("PLANNING PHASE:\n");
        //printf("Mouse At %d x %d\n",game->mouse.x, game->mouse.y);
        SnakePart *current = game->head;
        //printf("Snake Head at %d x %d\n", current->x, current->y);
        while(current->next != NULL){
            current = current->next;
            //printf("Snake Body Part at %d x %d\n", current->x, current->y);
        }
        plan_index = 0;
        plan_len = 0;
        make_plan();
        //printf("Completed plan of length %d\n", plan_len);
    }

    //printf("EXECUTING PLAN(%d)\n", plan_index);
    game->direction = plan[plan_index];
    ++plan_index;
}

void display()
{
    processAI();
    snake_tick(game);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float offsetX = 1.0f/(game->width/2);
    float offsetY = 1.0f/(game->height/2);
#if DRAW_PLAN
    // first draw the plan
    int i0 = 0;
    if (plan_index > game->score + 1) {
        i0 = plan_index - (game->score + 1);
    }
    for(int i=i0; i<plan_len; ++i) {
        int x_adjust = plan_squares[i].x - game->width/2;
        int y_adjust = plan_squares[i].y - game->height/2;
        float x = (float)(x_adjust)/(game->width/2);
        float y = (float)(y_adjust)/(game->height/2);
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(x,y);
        glVertex2f(x,y+offsetY);
        glVertex2f(x+offsetX,y+offsetY);
        glVertex2f(x+offsetX,y);
        glEnd();
    }
#endif 
    SnakePart * cur = game->head;
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
