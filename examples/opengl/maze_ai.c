//
// Implements the A* maze solving algorithm for the snake AI
//
#include <GL/glut.h>
#include "snake.h"
#include <stdio.h>

#define GAME_HEIGHT 512
#define GAME_WIDTH 512

const char title[] = "Snake";
const int windowWidth = GAME_WIDTH;
const int windowHeight = GAME_HEIGHT;
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

struct maze_node {
    int x;
    int y;
    int f_cost;
    int g_cost;
    int h_cost;
    struct maze_node *parent;

    // for linked listing
    struct maze_node *next;
    struct maze_node *prev;
};

// stores every possible node so we don't have to do that pesky mallocing
// also convenient because we can address nodes by their x-y values
static struct maze_node nodes[GAME_WIDTH * GAME_HEIGHT]; 

static struct maze_node *open = NULL; //the first entry in the open maze_node linked list
static struct maze_node *closed = NULL; //the first entry in the closed maze_node linked list
static struct maze_node *plan = NULL; //the result of the maze-solving agorithm, the planned moves 

// add a node to a linked list
void list_add(struct maze_node **start, struct maze_node *new) 
{
    // just tack the new one on the beginning that's the easiest thing to do
    new->next = *start;
    new->prev = NULL;
    if (*start != NULL) {
        (*start)->prev = new;
    }
    *start = new; 
}

// remove a node from a linked list
// assumes node is in the list, does not check
void list_remove(struct maze_node **start, struct maze_node *rem) 
{
    if (*start == rem) {
        // first node
        if ((*start)->next != NULL) {
            (*start)->next->prev = NULL;
        }
        *start = (*start)->next;

    } else {
        // not the first node
        if (rem->prev) {
            rem->prev->next = rem->next;
        }
        if (rem->next) {
            rem->next->prev = rem->prev;
        }
    }
}

// checks if a node already exists on a list 
int on_list(struct maze_node *start, struct maze_node *check) 
{
    if (!start) {
        return 0;
    }

    if (start == check) { 
        return 1;
    }

    struct maze_node *cur = start->next;
    while (cur != NULL) {
        if (cur == check) {
            return 1;
        }
        cur = cur->next;
    }

    return 0;
}

// gets the proper snake direction of travel between node a and node b
int get_direction(struct maze_node *a, struct maze_node *b) {
    int rc = DIRECTION_POS_X;
    if (a->x == b->x) {
        if (b->y == a->y + 1) {
            rc = DIRECTION_POS_Y;
        } else if (b->y == a->y - 1) {
            rc = DIRECTION_NEG_Y;
        } else {
            printf("IMPOSSIBLE STEP, NO DIRECTION\n");
        }
    } else if (a->y == b->y) {
        if (b->x == a->x + 1) {
            rc = DIRECTION_POS_X;
        } else if (b->x == a->x - 1) {
            rc = DIRECTION_NEG_X;
        } else {
            printf("IMPOSSIBLE STEP, NO DIRECTION\n");
        }
    } else {
        printf("IMPOSSIBLE STEP, NO DIRECTION\n");
    }

    return rc;
}

void print_list(struct maze_node *start) {
    while (start) {
        //printf("<%d, %d> ", start->x, start->y);
        start = start->next;
    }
    //printf("\n");
}


void find_path()
{
    while(open != NULL) {
        //printf("OPEN LIST: ");
        print_list(open);

        // find the minimum f_cost in open nodes
        struct maze_node *cur = open;
        struct maze_node *i = cur; // iterator
        int min_f = cur->f_cost;
        while(i->next != NULL) {
            i = i->next;
            if (i->f_cost < min_f) {
                cur = i;
                min_f = cur->f_cost;
            }
        }

        //printf("Current Node: <%d, %d>\n", cur->x, cur->y); 

        // move cur to closed list
        list_remove(&open, cur);
        list_add(&closed, cur);
        if (cur->x == game->mouse.x && cur->y == game->mouse.y) {
            // we are done, add the chosen path to the plan and exit
            list_add(&plan, cur);
            //printf("Added <%d, %d> to closed list! Path Found!\n", cur->x, cur->y);
            while (cur->parent != NULL) {
                cur = cur->parent;
                list_add(&plan, cur);
            }

            return;
        }


        // for each valid square ajacent to cur, add to open list
        // attempt to select 4 nodes in each direction
        struct maze_node *sel[4];
        for (int i=0; i<4; ++i) {
            sel[i] = NULL;
        }
        
        if (cur->x+1 < GAME_WIDTH && !partAtLater(cur->x+1, cur->y, cur->g_cost)) {
            sel[0] = &nodes[(cur->x+1) + cur->y*GAME_WIDTH];
            sel[0]->x = cur->x+1;
            sel[0]->y = cur->y;
        }
        if (cur->y+1 < GAME_HEIGHT && !partAtLater(cur->x, cur->y+1, cur->g_cost)) {
            sel[1] = &nodes[(cur->x) + (cur->y+1)*GAME_WIDTH];
            sel[1]->x = cur->x;
            sel[1]->y = cur->y+1;
        }
        if (cur->x-1 >= 0 && !partAtLater(cur->x-1, cur->y, cur->g_cost)) {
            sel[2] = &nodes[(cur->x-1) + cur->y*GAME_WIDTH];
            sel[2]->x = cur->x-1;
            sel[2]->y = cur->y;
        }
        if (cur->y-1 >= 0 && !partAtLater(cur->x, cur->y-1, cur->g_cost)) {
            sel[3] = &nodes[(cur->x) + (cur->y-1)*GAME_WIDTH];
            sel[3]->x = cur->x;
            sel[3]->y = cur->y-1;
        }

        for (int i=0; i<4; ++i) {
            if (sel[i]) {
                // ignore if on closed list
                if (!on_list(closed, sel[i])) {
                    if (on_list(open, sel[i])) {
                        //printf("<%d, %d> already on list, updating\n", sel[i]->x, sel[i]->y);
                        // update the node
                        if (sel[i]->g_cost > cur->g_cost + 1) {
                            sel[i]->g_cost = cur->g_cost + 1;
                            sel[i]->parent = cur;
                            sel[i]->f_cost = cur->h_cost + cur->g_cost;
                        }
                    } else {
                        // initialize the node
                        //printf("<%d, %d> not on open list, adding\n", sel[i]->x, sel[i]->y);
                        sel[i]->parent = cur;
                        sel[i]->g_cost = cur->g_cost + 1; // TODO make the cost of changing direction higher
                        sel[i]->h_cost = abs(sel[i]->x - game->mouse.x) + abs(sel[i]->y - game->mouse.y);
                        sel[i]->f_cost = sel[i]->g_cost + sel[i]->h_cost;
                        sel[i]->prev = NULL;
                        sel[i]->next = NULL;
                        list_add(&open, sel[i]);
                    }
                }
            }
        }
    }

    //printf("No open nodes, no path possible, exiting\n");
}

void processAI()
{
    if (plan == NULL) {
        // reset lists
        open = NULL;
        closed = NULL;

        // initialize first node
        struct maze_node *cur = &nodes[game->head->x + GAME_WIDTH * game->head->y];
        cur->x = game->head->x;
        cur->y = game->head->y;
        cur->parent = NULL;
        cur->next = NULL;
        cur->prev = NULL;
        cur->h_cost = abs(cur->x - game->mouse.x) + abs(cur->y - game->mouse.y);
        cur->g_cost = 0;
        cur->f_cost = cur->h_cost + cur->g_cost;

        // add first node to open
        list_add(&open, cur);

        // run maze solving algorithm
        find_path();
    } 

    if (plan != NULL) {
        if (plan->next) {
            game->direction = get_direction(plan, plan->next);
            list_remove(&plan, plan);
            if (plan->next == NULL) {
                // no pairs of nodes left, we are done with the plan
                plan = NULL;
            }
        } else {
            printf("ERROR: plan contained only 1 node, cannot form direction to travel from plan\n");
        }
    } else {
        printf("No plan was made, goodbye cruel world\n");
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
