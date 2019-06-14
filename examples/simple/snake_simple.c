#include "snake.h"
#include <stdio.h>

enum SnakeDirection get_user_input_direction()
{
	char direction[2];
    enum SnakeDirection dir = DIRECTION_POS_X;
	printf("[u]p, [d]own, [l]eft, or [r]ight? ");
	scanf("%s", direction);
    switch(*direction) {
        case('r'):
            printf("right\n");
            dir = DIRECTION_POS_X;
            break;
        case('u'):
            printf("up\n");
            dir = DIRECTION_POS_Y;
            break;
        case('l'):
            printf("left\n");
            dir = DIRECTION_NEG_X;
            break;
        case('d'):
            printf("down\n");
            dir = DIRECTION_NEG_Y;
            break;
        default:
            printf("bad intput, default: right\n");
    }

    return dir;
}

int main()
{
  SnakeGame *game = snake_init(32, 32);
  while(!game->gameover){
    //display some information about the snake game
    printf("Mouse At %d x %d\n",game->mouse.x, game->mouse.y);
    SnakePart *current = game->head;
    printf("Snake Head at %d x %d\n", current->x, current->y);
    while(current->next != NULL){
      current = current->next;
      printf("Snake Body Part at %d x %d\n", current->x, current->y);
    }
    printf("The score is %d\n", game->score);
    
    //get user input 
    game->direction = get_user_input_direction(); //Should return a SnakeDirection enum
    
    //process a game tick
    snake_tick(game);
  }

  printf("Game over, final score: %d\n", game->score);
  return 0;
} 
