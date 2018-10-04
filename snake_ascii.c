#include <curses.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "snake.h"

//this is BLATANT PLAGARISM from the source code of sl (steam locomotive)
int my_mvaddstr(int y, int x, char *str)
{
    for ( ; x < 0; ++x, ++str)
            if (*str == '\0')  return ERR;
    for ( ; *str != '\0'; ++str, ++x)
        if (mvaddch(y, x, *str) == ERR)  return ERR;
    return OK;
}
void processAI(){
    if(head->x != mouse.x || head->y != mouse.y){ //do nothing if you just ate
        if(head->x < mouse.x){
            direction = 0; //right
        }
        else if (head->x > mouse.x){
            direction = 2; //left 
        }
        else if (head->y < mouse.y){
            direction = 1; //up
        }
        else {
            direction = 3; //down 
        }
    }
}
int main()
{
    snake_init();

    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    leaveok(stdscr, TRUE);
    scrollok(stdscr, FALSE);
    while(1)
    {
        //clear old pixels before updating and showing new ones
        my_mvaddstr(mouse.x,mouse.y," ");
        cur = head;
        while(1)
        {
            my_mvaddstr(cur->x,cur->y," ");
            if(cur->next) cur = cur->next;
            else break;
        }
        processAI();
        snake();
        my_mvaddstr(mouse.x,mouse.y,"M");
        cur = head;
        while(1)
        {
            my_mvaddstr(cur->x,cur->y,"S");
            if(cur->next) cur = cur->next;
            else break;
        }
        getch();
        refresh();
        usleep(100000);
    }
    endwin();
    return 0;
}
