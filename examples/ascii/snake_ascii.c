#include "curses.h"
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "snake.h"

//#define USE_AI

static SnakeGame * game;

int part_at(int x, int y)
{
    SnakePart * cur = game->head;
    while(1){
        if(cur->x == x && cur->y == y)  return 1;
        if(!cur->next) return 0;
        cur = cur->next;
    }
}
void process_ai()
{
    //check to see which directions are valid
    int invalid[4];
    invalid[0]= part_at(game->head->x+1,game->head->y);  //right
    invalid[1]= part_at(game->head->x,game->head->y+1);  //up
    invalid[2]= part_at(game->head->x-1, game->head->y); //left
    invalid[3]= part_at(game->head->x, game->head->y-1); //down
    
    //start by setting the direction to any valid one
    int i=0;
    while(invalid[game->direction]) {
        game->direction = (game->direction+1)%4;
        if(++i >= 3) break;
    }
    if(!invalid[0] && game->head->x < game->mouse.x){
        game->direction = 0; //right
    }
    else if (!invalid[2] && game->head->x > game->mouse.x){
        game->direction = 2; //left 
    }
    else if (!invalid[1] && game->head->y < game->mouse.y){
        game->direction = 1; //up
    }
    else if (!invalid[3]) {
        game->direction = 3; //down 
    }
}
int main()
{
    game = snake_init(32, 32);

    //setup curses
    initscr();
    start_color();
    use_default_colors();
    noecho();
    keypad(stdscr,TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE);
    leaveok(stdscr, TRUE);
    scrollok(stdscr, FALSE);
    int use_colors = has_colors();
    if(use_colors)
    {
        init_pair(1, COLOR_RED, COLOR_RED);
        init_pair(2, COLOR_WHITE, COLOR_WHITE);
        init_pair(3, -1, -1);
    }
    //done setting up curses


    //begin display loop
    while(1)
    {
        //clear old pixels before updating and showing new ones
        if(use_colors) attron(COLOR_PAIR(3));
        mvaddch(game->mouse.y,2*game->mouse.x,' ');
        mvaddch(game->mouse.y,2*game->mouse.x+1,' ');
        SnakePart * cur = game->head;
        while(1)
        {
            mvaddch(cur->y,2*cur->x,' ');
            mvaddch(cur->y,2*cur->x+1,' ');
            if(cur->next) cur = cur->next;
            else break;
        }
        if(use_colors) attron(COLOR_PAIR(1));


        //get player or ai input
#ifdef USE_AI
        process_ai();
#else
        int ch = getch();
        //NOTE: Up and down are flipped since snake doesn't use 'screen' coordinates
        switch(ch){
            case KEY_RIGHT:
                game->direction = 0;
                break;
            case KEY_UP:
                game->direction = 3;
                break;
            case KEY_LEFT:
                game->direction = 2;
                break;
            case KEY_DOWN:
                game->direction = 1;
                break;
        }
#endif

        // process game tick
        snake_tick(game);

        // process game events
        if(game->gameover)
        {
            if(use_colors) attron(COLOR_PAIR(3));
            char result[50];
            sprintf(result, "Game over! Score: %i      ", game->score);
            mvaddstr(game->height+1,0, result);
        }

        //draw new new pixels
        mvaddch(game->mouse.y,2*game->mouse.x,'<');
        mvaddch(game->mouse.y,2*game->mouse.x+1,'>'); 
        if(use_colors) attron(COLOR_PAIR(2));
        cur = game->head;
        while(1)
        {
            mvaddch(cur->y,2*cur->x,'[');
            mvaddch(cur->y,2*cur->x+1,']');
            if(cur->next) cur = cur->next;
            else break;
        }
        getch();
        refresh();
        usleep(60000);
    }
    endwin();
    return 0;
}
