#include <string>

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

    move(5, 5);

    std::string text = "Hello world!";
    for(int i = 0; i < text.size(); i++) {
        addch(text[i]);
        addch(' ');
    }

    refresh();

    while(1);

    endwin();
    return 0;
}
