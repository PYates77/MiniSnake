# MiniSnake #

A very small and simple library for creating custom snake games. 

## snake.h ##

The core of MiniSnake is header-only library snake.h. This header contains all the information required to run a snake game, except how to display it on a screen.
All the relevant snake game data is contained within the SnakeGame struct, and global variables have been avoided, so multiple games of snake could theoretically run simultaneously.

```c
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
    unsigned int width;
    unsigned int height;
    /* signal that the snake died, useful if you want to display the score or end the game */
    uint8_t gameover; 
    /* signals mouse eaten event, useful if you want to update the score or play an animation */
    uint8_t mouse_eaten; 
} SnakeGame;
 ```
 
 The snake is formed by a doubly-linked list of SnakeParts, which each contain an x-y location for the SnakePart. The entire snake can be accessed by traversing the list starting at the head pointer in the SnakeGame struct.
 
 ```c
 /* parts form a doubly-linked list to create snake body */
 typedef struct part
 {     
     unsigned int x;
     unsigned int y;
     struct part* next;
     struct part* prev;
 } SnakePart;
```

Oh and if opening snake.h to read it is too difficult, here's the only other thing you need to know:

```c
enum SnakeDirection
{
     DIRECTION_POS_X = 0,
     DIRECTION_POS_Y = 1,
     DIRECTION_NEG_X = 2,
     DIRECTION_NEG_Y = 3
 };
 ```

## Running a game of snake ##

Playing a game of snake is super simple: 

* Create a snake game with some playing field size
* In a loop, call the snake\_tick() function with the pointer to the SnakeGame to process one tick of the game.
* In-between calls to the snake\_tick() function, read the location of the mouse and body parts, and control the direction of the snake's head.
* Optionally, read the score and the gameover flag

```c
#include "snake.h"
void super_simple_snake_game(){
  SnakeGame * game = snake_init(32, 32);
  while(!game->gameover){
    //display some information about the snake game
    printf("Mouse At %d x %d\n",game->mouse.x, game->mouse.y);
    SnakePart * current = game->head;
    printf("Snake Head at %d x %d\n", current->x, current->y);
    while(current->next != NULL){
      current = current->next;
      printf("Snake Body Part at %d x %d\n", current.x, current.y);
    }
    printf("The score is %d\n", game->score);
    
    //get user input 
    game->direction = get_user_input_direction(); //should be 0 through 3 for 4 different possible directions
    
    //process a game tick
    snake_tick(game);
  }
} 
```

## Examples ##

Unless you enjoy playing text-only versions of snake, chances are you'll probably want to have some code that can display the game in a friendly, human-viewable manner. Chances are also pretty good that you're too lazy to do that from scratch, so navigate over to the examples directory and do the regular cmake thing to build one of the examples:

```bash
cd examples/ascii
mkdir build && cd build
cmake ..
make
./snake_ascii
```

### ASCII Snake ###
You'll need to install ncurses-dev, so for ubuntu that would be 

```bash
sudo apt-get install libncurses-dev
```

If you use some other distro that doesn't use apt I'm sure you can figure it out.

### OpenGL Snake ###

You will need: GLEW (libglew-dev) and GLUT (freeglut3-dev,libxmu-dev, libxi-dev) 

I also toyed with creating snake playing AI's in this folder, so you can either ignore those or try to improve upon them because, let's be honest, you're a better programmer than me and you can probably figure out a simple snake-playing AI.
