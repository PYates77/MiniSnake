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

void snake_init(){ //set up all variables and memory
    srand(time(NULL));
    head = (SnakePart *)malloc(sizeof(SnakePart));
    head->x = WIDTH>>1;
    head->y = HEIGHT>>1;
    head->next = NULL;
    head->prev = NULL;
    mouse.x = rand()%WIDTH;
    mouse.y = rand()%HEIGHT;
    tail = head;
    cur = tail;
    cur->next = head;
    head->prev = cur;
    tail = tail->prev;
    tail->next = NULL;
    cur->prev = NULL;
    score = 0;
    direction = 0;
}
void snake_reset(){ //delete all non-head body parts, reset variables
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
    // CHECK MOUSE EATEN EVENT //
    if(flag){
        cur = (SnakePart*)malloc(sizeof(SnakePart));
        cur->next = head;
        head->prev = cur;
        ++score;
        flag = 0; //reset flag
    }
    else{
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
            cur->x=(cur->x+1);
            break;
        case 1:
            cur->y=(cur->y+1);
            break;
        case 2:
            cur->x=(cur->x-1);
            break;
        case 3:
            cur->y=(cur->y-1);
            break;
    }
    cur->x=cur->x%WIDTH;
    cur->y=cur->y%HEIGHT;
    head = cur;
    
    //printf("Head: (%i, %i) Mouse: (%i,%i)\n",head->x,head->y, mouse.x, mouse.y);
    // MOUSE EATEN EVENT //
    if(head->x == mouse.x && head->y == mouse.y){ //mouse is guaranteed not to spawn in a snakepart location
        do {
            mouse.x = rand()%WIDTH;
            mouse.y = rand()%HEIGHT;
            cur = head;
            flag = 0; //flag being used for collision detection
            do {
                flag = (mouse.x == cur->x && mouse.y == cur->y);
                if(!cur->next) break;
                else cur = cur->next;
            } while ( flag == 0 );
        } while ( flag == 1);
        //make a new snake part and put it in front of the head
        flag = 1; //flag being used for mouse eaten event 
    }
    // MOUSE NOT EATEN EVENT //
    else { // need to check for body collisions
        cur = head;
        while(cur->next){
            cur = cur->next;
            if(cur->x == head->x && cur->y == head->y){ //collision occurred 
                //GAME OVER CONDITION
                snake_reset();
                break;
            }
        }
    }
}

