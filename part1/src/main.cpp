#include <curses.h>
#include <ncurses/ncurses.h>

// #include <quiz.h>

int r,c, nrows, ncols;

void draw(char dc){
    move(r,c);
    delch();
    refresh();
    r++;
    if(r == nrows){
        r = 0;
        c++;
        if(c == ncols) c= 0;
    }
    
}

int main(int argc, char **argv){
    // parse 
    // quiz q;
    // quiz();
    int i; char d;
    WINDOW *wnd;
    wnd = initscr();
    cbreak();
    noecho();
    getmaxyx(wnd,nrows,ncols);
    clear();
    refresh();
    
    r= 0; c = 0;
    while(1) {
        d = getch();
        if(d == 'q') break;
        draw(d);
    
    }
    endwin();
    return 0;
}
