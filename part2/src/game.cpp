#include <string>
#include <ncurses.h>

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

    move(5, 5);

    std::string text = "Hello world!";
    for(unsigned int i = 0; i < text.size(); i++) {
        addch(text[i]);
        addch(' ');
    }

    refresh();

    while(1);
}
