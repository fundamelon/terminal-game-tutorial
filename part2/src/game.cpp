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
    setFrame();

    // enable function keys
    keypad(wnd, true);

    // disable input blocking
    nodelay(wnd, true);

    // enable color
    if(!has_colors()) {
        endwin();
        printf("ERROR: Terminal does not support color.\n");
        exit(1);
    }

    start_color();

    setColorscheme(COLOR_BLACK, COLOR_CYAN);

    return 0;
}



void run() {

    player.disp_char = '0';
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

        //nanosleep({0, 1000000000}, NULL);
        usleep(10000);
    };

    endwin();
}


void setColorscheme(short fg, short bg) {
    init_pair(1, fg, bg);
    wbkgd(wnd, COLOR_PAIR(1));
}

void setFrame(){
    // creates simple frame around window composed of vertical and horizontal lines
    box(wnd, 0, 0);
    
    // border characters can be set manually using the border function
    // border( wnd, leftside, rightside, topside, bottom side, tlcorner, 
    //                                      trcorner, blcorner, brcorner);
}
