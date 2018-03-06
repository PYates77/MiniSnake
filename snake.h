#ifndef MINISNAKE_SNAKE_H
#define MINISNAKE_SNAKE_H
typedef struct part{ //parts form a doubly-linked list to create snake body
    unsigned char x;
    unsigned char y;
    struct part* next;
    struct part* prev;
} SnakePart;
static const unsigned char WIDTH=32; // MUST BE A POWER OF 2 
static const unsigned char HEIGHT=32; // MUST BE A POWER OF 2
unsigned char direction; //gives direction of head (2-bit) RIGHT=0, UP=1, LEFT=2, DOWN=3
unsigned int score; 
SnakePart *head;
SnakePart *tail;
SnakePart mouse; //convenient to use snakepart for mouse, but mouse doesn't need next* or prev*
SnakePart *cur; //used for iteration, defined globally so it doesn't have to be redefined later
unsigned char collision; // collision flag, defined globally for reuse and so it doesn't have to be redefined later

extern inline void snake_init();
extern inline void snake_reset();
void snake(void);

#endif //MINISNAKE_SNAKE_H
