#include <time.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct part{ //parts form a doubly-linked list to create snake body
    unsigned char x;
    unsigned char y;
    struct part* next;
    struct part* prev;
} SnakePart;
static const unsigned char WIDTH=32; // MUST BE A POWER OF 2 
static const unsigned char HEIGHT=32; // MUST BE A POWER OF 2
unsigned char direction = 0; //gives direction of head (2-bit) RIGHT=0, UP=1, LEFT=2, DOWN=3
SnakePart *head;
SnakePart *tail;
SnakePart mouse; //convenient to use snakepart for mouse, but mouse doesn't need next* or prev*
SnakePart *cur; //used for iteration, defined globally so it doesn't have to be redefined later
unsigned char collision; // collision flag, defined globally for reuse and so it doesn't have to be redefined later


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

inline void reset(){
    printf("Snake Died! Time to reset!\n");
    direction = 0;
    cur = head;
    while(cur->next){
        cur = cur->next;
        if(cur != tail){
            free(cur);
        }
    }
    head->x = WIDTH>>1;
    head->y = HEIGHT>>1;
    head->next = NULL;
    head->prev = NULL;
    mouse.x = rand()%WIDTH;
    mouse.y = rand()%HEIGHT;
    tail = head;
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
    }
    else { //mouse not eaten - need to check for body collisions
        cur = head;
        while(cur->next){
            cur = cur->next;
            if(cur->x == head->x && cur->y == head->y){
                //GAME OVER CONDITION
                reset();
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

