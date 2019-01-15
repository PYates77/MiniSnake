#ifndef MINISNAKE_SNAKE_H
#define MINISNAKE_SNAKE_H

#include <stdint.h>
#include <time.h>

enum SnakeDirection
{
    DIRECTION_POS_X = 0,
    DIRECTION_POS_Y = 1,
    DIRECTION_NEG_X = 2,
    DIRECTION_NEG_Y = 3
};

/* parts form a doubly-linked list to create snake body */
typedef struct part
{     
    unsigned int x;
    unsigned int y;
    struct part* next;
    struct part* prev;
} SnakePart;

/* holds game info so we can pass the pointer as a handle */
typedef struct game
{ 
    /* gives direction of head */
    enum SnakeDirection direction; 
    /* The number of mice eaten since the snake died last */
    unsigned int score; 
    SnakePart *head;
    SnakePart *tail;
    /* convenient to use snakepart for mouse, but mouse doesn't need the next or prev pointers */
    SnakePart mouse; 
    /* The playing field width and height */
    unsigned int  width;
    unsigned int  height;
    /* signal that the snake died, useful if you want to display the score or end the game */
    uint8_t gameover; 
    /* signals mouse eaten event, useful if you want to update the score or play an animation */
    uint8_t mouse_eaten; 
} SnakeGame;


extern inline SnakeGame * snake_init(unsigned int width, unsigned int height)
{ 
    //set up all variables and memory
    SnakeGame * game = (SnakeGame *)malloc(sizeof(SnakeGame));
    game->width = width;
    game->height = height;
    game->gameover = 0;
    SnakePart * cur;
    srand(time(NULL));
    game->head = (SnakePart *)malloc(sizeof(SnakePart));
    game->head->x = game->width>>1;
    game->head->y = game->height>>1;
    game->head->next = NULL;
    game->head->prev = NULL;
    game->mouse.x = rand()%game->width;
    game->mouse.y = rand()%game->height;
    game->tail = game->head;
    cur = game->tail;
    cur->next = game->head;
    game->head->prev = cur;
    game->tail = game->tail->prev;
    game->tail->next = NULL;
    cur->prev = NULL;
    game->score = 0;
    game->direction = DIRECTION_POS_X;
    return game;
}
extern inline void snake_reset(SnakeGame * game){ //delete all non-head body parts, reset variables
    //printf("Snake Died! Score: %i\n",score);
    game->gameover = 0;
    SnakePart * cur = game->head->next;
    if(cur != NULL)
    {
        SnakePart * temp;
        while(1){
            temp = cur->next;
            free(cur);
            if(temp) cur=temp;
            else break;
        }
    }
    game->head->x = game->width>>1;
    game->head->y = game->height>>1;
    game->head->next = NULL;
    game->head->prev = NULL;
    game->mouse.x = rand()%game->width;
    game->mouse.y = rand()%game->height;
    game->tail = game->head;
    cur = game->tail;
    cur->next = game->head;
    game->head->prev = cur;
    game->tail = game->tail->prev;
    game->tail->next = NULL;
    cur->prev = NULL;
    game->score = 0;
    game->direction = DIRECTION_POS_X;
}

//Each game tick follows this process
//// MOVE /////
// Check for mouse eaten - head location == mouse location
// IF: no mouse eaten just before - move the tail snakepart to the correct new head location 
// IF: mouse just eaten - new snake part in correct new head location - move mouse to new location 
// Using screen wrap around logic for now
// update head*
//// CHECK COLLISION ////
// Check head location against all body parts //
// IF: collision - reset game //
extern inline void snake_tick(SnakeGame * game)
{
    if(game->gameover){
        snake_reset(game);
    }
    // CHECK MOUSE EATEN EVENT //
    SnakePart * cur;
    if(game->mouse_eaten){
        cur = (SnakePart*)malloc(sizeof(SnakePart));
        cur->next = game->head;
        game->head->prev = cur;
        ++game->score;
        game->mouse_eaten = 0; //reset mouse eaten flag
    }
    else{
        cur = game->tail;
        cur->next = game->head;
        game->head->prev = cur;
        game->tail = game->tail->prev;
        game->tail->next = NULL;
        cur->prev = NULL;
    }
    // MOVE //
    cur->x = game->head->x;
    cur->y = game->head->y;
    switch(game->direction){
        case DIRECTION_POS_X:
            cur->x=(cur->x+1);
            break;
        case DIRECTION_POS_Y:
            cur->y=(cur->y+1);
            break;
        case DIRECTION_NEG_X:
            cur->x=(cur->x-1);
            break;
        case DIRECTION_NEG_Y:
            cur->y=(cur->y-1);
            break;
    }
    cur->x=cur->x%game->width;
    cur->y=cur->y%game->height;
    game->head = cur;
    
    //printf("Head: (%i, %i) Mouse: (%i,%i)\n",head->x,head->y, mouse.x, mouse.y);
    // MOUSE EATEN EVENT //
    //mouse is guaranteed not to spawn in a snakepart location
    if(game->head->x == game->mouse.x && game->head->y == game->mouse.y){ 
        uint8_t flag;
        //set mouse to random location until this location does not contain a snakepart
        do {
            game->mouse.x = rand()%game->width;
            game->mouse.y = rand()%game->height;
            cur = game->head;
            flag = 0; 
            do {
                flag = (game->mouse.x == cur->x && game->mouse.y == cur->y); 
                if(!cur->next) break;
                else cur = cur->next;
            } while ( flag == 0 );
        } while ( flag == 1);

        // mouse eaten event will be handled in the next game tick
        game->mouse_eaten = 1; 
    }

    // MOUSE NOT EATEN EVENT //
    // need to check for body collisions 
    else { 
        cur = game->head;
        while(cur->next){
            cur = cur->next;
            if(cur->x == game->head->x && cur->y == game->head->y){ //collision occurred 
                //GAME OVER CONDITION
                game->gameover = 1;
                break;
            }
        }
    }
}


#endif //MINISNAKE_SNAKE_H
