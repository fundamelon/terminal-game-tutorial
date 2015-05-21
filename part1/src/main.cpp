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


    endwin();
    return 0;
}
