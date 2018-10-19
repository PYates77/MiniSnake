#ifndef MINISNAKE_SNAKE_H
#define MINISNAKE_SNAKE_H

typedef struct part{ //parts form a doubly-linked list to create snake body
    unsigned char x;
    unsigned char y;
    struct part* next;
    struct part* prev;
} SnakePart;

typedef struct game{ //holds game info so we can pass the pointer as a handle
    unsigned char direction; //gives direction of head (2-bit) RIGHT=0, UP=1, LEFT=2, DOWN=3
    unsigned int score; 
    SnakePart *head;
    SnakePart *tail;
    SnakePart mouse; //convenient to use snakepart for mouse, but mouse doesn't need next* or prev*
    unsigned char flag; // multipurpose flag
    unsigned char width;
    unsigned char height;
    unsigned char gameover; // signal that the snake died, useful if you want to display the score or end the game
} SnakeGame;

SnakeGame * snake_init(unsigned char width, unsigned char height);
void snake_reset(SnakeGame * game);
void snake(SnakeGame *);

#endif //MINISNAKE_SNAKE_H
