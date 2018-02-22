#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
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
SnakePart mouse;
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
        printf("Head: (%i, %i) Mouse: (%i,%i)\n",head->x,head->y, mouse.x, mouse.y);
        if(head->x == mouse.x && head->y == mouse.y){ //mouse eaten. mouse is guaranteed not to be in a snakepart location
            printf("Mouse Eaten\n");
            do {
               printf("searching for mouse position\n");
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
            cur = (SnakePart*)malloc(sizeof(SnakePart));
            cur->next = head;
            head->prev = cur; 
            printf("done handling mouse eaten\n");
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


///////////OPENGL///////////////
const char title[] = "Snake";
const int windowWidth = 512;
const int windowHeight = 512;
const int refreshMillis = 50;
const int screenOffsetX = 300;
const int screenOffsetY = 100;
float temp = 0;
void initGL(){
    glClearColor(0.0,0.0,0.0,1.0); //black background
}
void specialKeys(int key, int mouseX, int mouseY){
    switch(key){
        case GLUT_KEY_RIGHT:
            direction = 0;
            break;
        case GLUT_KEY_UP:
            direction = 1;
            break;
        case GLUT_KEY_LEFT:
            direction = 2;
            break;
        case GLUT_KEY_DOWN:
            direction = 3;
            break;
    }
}
void display(){
    snake(); 
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    cur = head;
    float offsetX = 1.0f/(WIDTH/2);
    float offsetY = 1.0f/(HEIGHT/2);
    glBegin(GL_QUADS);
        float x = (float)(mouse.x-HEIGHT/2)/(HEIGHT/2);
        float y = (float)(mouse.y-HEIGHT/2)/(HEIGHT/2);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(x,y);
        glVertex2f(x,y+offsetY);
        glVertex2f(x+offsetX,y+offsetY);
        glVertex2f(x+offsetX,y);     
    glEnd();
    while(1){
        float x = (float)(cur->x-HEIGHT/2)/(HEIGHT/2);
        float y = (float)(cur->y-HEIGHT/2)/(HEIGHT/2); 
        glBegin(GL_QUADS);
            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex2f(x, y);
            glVertex2f(x, y+offsetY);
            glVertex2f(x+offsetX, y+offsetY);
            glVertex2f(x+offsetX, y);
        glEnd();
        if(cur->next) cur = cur->next;
        else break;
    }

    glFlush();

    
}
void Timer(int value){
    glutPostRedisplay();
    glutTimerFunc(refreshMillis, Timer,0);
}
int main(int argc, char** argv){
    printf("Hello Snake World!\n");
    srand(time(NULL));
    head = (SnakePart*)malloc(sizeof(SnakePart));
    head->x = WIDTH>>1;
    head->y = HEIGHT>>1;
    head->next = NULL;
    head->prev = head;
    tail = head;
    mouse.x = rand()%WIDTH;
    mouse.y = rand()%HEIGHT;

    glutInit(&argc, argv);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(screenOffsetX,screenOffsetY);
    glutCreateWindow(title);
    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, Timer, 0);
    initGL();
    printf("AND GO!\n");
    glutMainLoop();
    return 0; 
}
