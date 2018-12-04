#ifndef MINISNAKE_SNAKE_H
#define MINISNAKE_SNAKE_H

#include <stdint.h>

typedef enum {
    DIRECTION_POS_X = 0,
    DIRECTION_POS_Y = 1,
    DIRECTION_NEG_X = 2,
    DIRECTION_NEG_Y = 3
} SnakeDirection;

typedef struct part{ //parts form a doubly-linked list to create snake body
    uint8_t x;
    uint8_t y;
    struct part* next;
    struct part* prev;
} SnakePart;

typedef struct game{ //holds game info so we can pass the pointer as a handle
    /*gives direction of head*/
    SnakeDirection direction; 
    /*The number of mice eaten since the snake died last*/
    unsigned int score; 
    SnakePart *head;
    SnakePart *tail;
    /*convenient to use snakepart for mouse, but mouse doesn't need the next or prev pointers*/
    SnakePart mouse; 
    /*The playing field width and height*/
    uint8_t width;
    uint8_t height;
    /*signal that the snake died, useful if you want to display the score or end the game*/
    uint8_t gameover; 
    /*multipurpose flag, signals mouse eaten or snake collision*/
    uint8_t flag; 
} SnakeGame;

SnakeGame * snake_init(uint8_t width, uint8_t height);
void snake_reset(SnakeGame * game);
void snake(SnakeGame *);

#endif //MINISNAKE_SNAKE_H
