#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "snake.h"

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

extern inline void snake_init(){
    head = (SnakePart *)malloc(sizeof(SnakePart));
    head->x = WIDTH>>1;
    head->y = HEIGHT>>1;
    head->next = NULL;
    head->prev = NULL;
    mouse.x = rand()%WIDTH;
    mouse.y = rand()%HEIGHT;
    tail = head;
    score = 0;
    direction = 0;
}
extern inline void snake_reset(){ //delete all non-head body parts, reset variables
    printf("Snake Died! Score: %i\n",score);
    cur = head;
    SnakePart* temp;
    while(1){
        temp = cur->next;
        free(cur);
        if(temp) cur=temp;
        else break;
    }
    snake_init();
}

void snake(){
    //printf("Head: (%i, %i) Mouse: (%i,%i)\n",head->x,head->y, mouse.x, mouse.y);
    if(head->x == mouse.x && head->y == mouse.y){ //mouse eaten. mouse is guaranteed not to spawn in a snakepart location
        do {
            mouse.x = rand()%WIDTH;
            mouse.y = rand()%HEIGHT;
            cur = head;
            collision = 0;
            do {
                collision = (mouse.x == cur->x && mouse.y == cur->y);
                if(!cur->next) break;
                else cur = cur->next;
            } while ( collision == 0 );
        } while ( collision == 1);
        //make a new snake part and put it in front of the head
        cur = (SnakePart*)malloc(sizeof(SnakePart));
        cur->next = head;
        head->prev = cur;
        ++score;
    }
    else { //mouse not eaten - need to check for body collisions
        cur = head;
        while(cur->next){
            cur = cur->next;
            if(cur->x == head->x && cur->y == head->y){
                //GAME OVER CONDITION
                snake_reset();
                break;
            }
        }
        cur = tail;
        cur->next = head;
        head->prev = cur;
        tail = tail->prev;
        tail->next = NULL;
        cur->prev = NULL;
    }
    // MOVE //
    cur->x = head->x;
    cur->y = head->y;
    switch(direction){
        case 0:
            cur->x=(head->x+1);
            break;
        case 1:
            cur->y=(head->y+1);
            break;
        case 2:
            cur->x=(head->x-1);
            break;
        case 3:
            cur->y=(head->y-1);
            break;
    }
    cur->x=cur->x%WIDTH;
    cur->y=cur->y%HEIGHT;
    head = cur;
    
}

