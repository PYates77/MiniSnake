#ifndef MINISNAKE_SNAKE_H
#define MINISNAKE_SNAKE_H

#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>

enum SnakeDirection{
    DIRECTION_POS_X = 0,
    DIRECTION_POS_Y = 1,
    DIRECTION_NEG_X = 2,
    DIRECTION_NEG_Y = 3
};

/* parts form a doubly-linked list to create snake body */
typedef struct part{     
    uint8_t x;
    uint8_t y;
    struct part* next;
    struct part* prev;
} SnakePart;

/* holds game info so we can pass the pointer as a handle */
typedef struct game{ 
    /* gives direction of head */
    enum SnakeDirection direction; 
    /* The number of mice eaten since the snake died last */
    unsigned int score; 
    SnakePart *head;
    SnakePart *tail;
    /* convenient to use snakepart for mouse, but mouse doesn't need the next or prev pointers */
    SnakePart mouse; 
    /* The playing field width and height */
    uint8_t width;
    uint8_t height;
    /* signal that the snake died, useful if you want to display the score or end the game */
    uint8_t gameover; 
    /* multipurpose flag, signals mouse eaten or snake collision */
    uint8_t flag; 
} SnakeGame;

//Each game tick follows this process
//// MOVE /////
// Check for mouse eaten - head location == mouse location
// IF: no mouse eaten just before - move the tail snakepart to the correct new head location 
// IF: mouse just eaten - new snake part in correct new head location - move mouse to new location 
// Using screen wrap around logic for now
// update head*
//// CHECK COLLISION ////
// Check head location against all body parts //
// IF: collision -reset game //

extern inline SnakeGame * snake_init(uint8_t width, uint8_t height){ //set up all variables and memory
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

extern inline void snake(SnakeGame * game){
    if(game->gameover){
        snake_reset(game);
    }
    // CHECK MOUSE EATEN EVENT //
    SnakePart * cur;
    if(game->flag){
        cur = (SnakePart*)malloc(sizeof(SnakePart));
        cur->next = game->head;
        game->head->prev = cur;
        ++game->score;
        game->flag = 0; //reset flag
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
    if(game->head->x == game->mouse.x && game->head->y == game->mouse.y){ //mouse is guaranteed not to spawn in a snakepart location
        do {
            game->mouse.x = rand()%game->width;
            game->mouse.y = rand()%game->height;
            cur = game->head;
            game->flag = 0; //flag being used for collision detection
            do {
                game->flag = (game->mouse.x == cur->x && game->mouse.y == cur->y);
                if(!cur->next) break;
                else cur = cur->next;
            } while ( game->flag == 0 );
        } while ( game->flag == 1);
        //make a new snake part and put it in front of the head
        game->flag = 1; //flag being used for mouse eaten event 
    }
    // MOUSE NOT EATEN EVENT //
    else { // need to check for body collisions
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
