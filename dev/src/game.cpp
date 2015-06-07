#include <unistd.h>
#include <ncurses.h>

#include <cstdint>
#include <string>

#include "game.h"



WINDOW* wnd;



int init() {

    wnd = initscr();
    cbreak();
    noecho();
    clear();
    refresh();

    return 0;
}



void run() {

    player.disp_char = 'o';
    player.pos = {6, 6};
    int in_char;
    bool exit_requested = false;
    curs_set(0);

    while(1) {
        in_char = wgetch(wnd);

        mvaddch(player.pos.x, player.pos.y, ' ');

        switch(in_char) {
            case 'q': 
                exit_requested = true; 
                break;
            case KEY_UP:
            case 'w':
                player.pos.x -= 1;
                break;
            case KEY_DOWN:
            case 's':
                player.pos.x += 1;
                break;
            case KEY_LEFT: 
            case 'a':
                player.pos.y -= 1; 
                break;
            case KEY_RIGHT: 
            case 'd':
                player.pos.y += 1; 
                break;
            default: 
                break;
        }

        mvaddch(player.pos.x, player.pos.y, player.disp_char);
        refresh();

        if(exit_requested) break;
    };

    endwin();
}


