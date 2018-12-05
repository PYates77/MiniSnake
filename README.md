# MiniSnake

A very small and simple library for creating custom snake games. 

## snake.h ##

The core of MiniSnake is header-only library snake.h. This header contains all the information required to run a snake game, except how to display it on a screen.
All the relevant snake game data is contained within the SnakeGame struct, and global variables have been avoided, so multiple games of snake could theoretically run simultaneously.

```c
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
 ```
 
 The snake formed by a doubly-linked list of SnakeParts, which simply contain a location. The entire snake can be accessed by traversing the list starting at the head pointer in the SnakeGame struct.
 
 ```c
 /* parts form a doubly-linked list to create snake body */
 typedef struct part{     
     uint8_t x;
     uint8_t y;
     struct part* next;
     struct part* prev;
 } SnakePart;
```

Oh and if opening snake.h is too difficult, here's the only other thing you need to know:

```c
enum SnakeDirection{
     DIRECTION_POS_X = 0,
     DIRECTION_POS_Y = 1,
     DIRECTION_NEG_X = 2,
     DIRECTION_NEG_Y = 3
 };
 ```

## Running a game of snake ##

Playing a game of snake is dead simple: 

* Create a snake game with some playing field size
* In a loop, call the snake() function with the pointer to the SnakeGame to process one tick of the game.
* In-between calls to the snake() function, read the location of the mouse and body parts, and control the direction of the snake's head.
* Optionally, read the score and the gameover flag

```c
#include "snake.h"
void super_simple_snake_game(){
  SnakeGame * game = snake_init(32, 32);
  while(!game->gameover){
    //display some information about the snake game
    printf("Mouse At %d x %d\n",game->mouse.x, game->mouse.y);
    SnakePart * current = game->head;
    printf("Snake Head at %d x %d\n", current.x, current.y);
    while(current->next != NULL){
      current = current->next;
      printf("Snake Body Part at %d x %d\n", current.x, current.y);
    }
    printf("The score is %d\n", game->score);
    
    //get user input 
    game->direction = get_user_input_direction(); //should be 0 through 3 for 4 different possible directions
    
    //process a game tick
    snake(game);
  }
} 
```

