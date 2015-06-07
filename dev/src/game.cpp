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
    int in_char;

    move(5, 5);
    std::string text = "loading...";
    
    for(unsigned int i = 0; i < text.size(); i++) {
        addch(text[i]);
        addch(' ');
    }

    refresh();

    while(1) {
        in_char = wgetch(wnd);
        if(in_char == 27) break;
        mvaddch(6, 5, in_char);
        refresh();
    };

    endwin();
}


