#include <string>
#include <ncurses.h>

#include "quiz.h"



WINDOW* wnd;



int init() {

    wnd = initscr();
    cbreak();
    noecho();
    clear();
    refresh();
}



int run() {

    move(5, 5);

    std::string text = "Hello world!";
    for(int i = 0; i < text.size(); i++) {
        addch(text[i]);
        addch(' ');
    }

    refresh();

    while(1);
}
