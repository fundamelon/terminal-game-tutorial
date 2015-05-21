#include <ncurses.h>

// #include <quiz.h>



int main(int argc, char **argv){
    // parse 
    // quiz q;
    // quiz();
    WINDOW *wnd;
    wnd = initscr();
    cbreak();
    noecho();
    clear();
    refresh();

    while(1);

    endwin();
    return 0;
}
