#include <unistd.h>
#include <ncurses.h>

#include <cstdint>
#include <string>

#include "game.h"



WINDOW* wnd;


struct {
    vec2i pos;
    vec2i dir;
    vec2ui bounds;
    char disp_char;
    char ship_type;
} player;


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

    setColorscheme(COLOR_WHITE, COLOR_BLACK);

    return 0;
}



void run() {


    vec2ui cur_size = { 0, 0 };
    getmaxyx(wnd, cur_size.x, cur_size.y);

    // define area for movement
    rect game_area = { { 2, 1}, { 74, 20 } };

    // define area for screen (default terminal size)
    rect screen_area = { { 0, 0 }, { 80, 24 } };

    // set screen size accordingly
    wresize(wnd, screen_area.height(), screen_area.width());

    
    player.disp_char = 'o';
    player.pos = {20, 20};
    player.bounds = { 3, 2 }; // player is 3 wide, 2 tall

    int in_char;
    bool exit_requested = false;
    curs_set(0);

    while(1) {

        //werase(wnd);
        move(0, 0);
        for(int i = 0; i < 80 * 24; i++) addch(' ');
        setFrame();
   
        // TODO: Give warning message if screen is too small!
        if(cur_size.x > screen_area.width() || cur_size.y > screen_area.height()) {}
        //winResize(cur_width, cur_height);
    
        in_char = wgetch(wnd);
        in_char = tolower(in_char);

        switch(in_char) {
            case 'q': 
                exit_requested = true; 
                break;
            case KEY_UP:
            case 'w':
                if(player.pos.y > game_area.top())
                    player.pos.y -= 1;
                break;
            case KEY_DOWN:
            case 's':
                if(player.pos.y <= game_area.bot())
                    player.pos.y += 1;
                break;
            case KEY_LEFT: 
            case 'a':
                if(player.pos.x > game_area.left())
                    player.pos.x -= 1;
                break;
            case KEY_RIGHT: 
            case 'd':
                if(player.pos.x <= game_area.right())
                    player.pos.x += 1; 
                break;
            default: 
                break;
        }

        mvaddch(player.pos.y, player.pos.x, player.disp_char); // (y, x)
        attron(A_ALTCHARSET);
        mvaddch(player.pos.y + 1, player.pos.x, ACS_UARROW);
        //mvaddch(player.pos.y - 1, player.pos.x, ACS_UARROW);
        mvaddch(player.pos.y, player.pos.x - 1, ACS_LARROW);
        mvaddch(player.pos.y, player.pos.x + 1, ACS_RARROW);
        attroff(A_ALTCHARSET);


        refresh();

        if(exit_requested) break;

        //nanosleep({0, 1000000000}, NULL);
        usleep(10000); // 1 ms
    };

    endwin();
}



void setColorscheme(short fg, short bg) {
    init_pair(1, fg, bg);
    wbkgd(wnd, COLOR_PAIR(1));
}



void setFrame(){
    // creates simple frame around window composed of vertical and horizontal lines
    attron(A_BOLD);
    box(wnd, 0, 0);
    attroff(A_BOLD);
    
    // border characters can be set manually using the border function
    // border( wnd, leftside, rightside, topside, bottom side, tlcorner, 
    //                                      trcorner, blcorner, brcorner);
}



void winResize(int &orig_width, int &orig_height){
    int new_width, new_height;

    getmaxyx(wnd, new_width, new_height); 

    // if window dimensions have changed, update border
    if(new_width != orig_width || new_height != orig_height){
        orig_width = new_width;
        orig_height = new_height;
        
        wresize(wnd, new_height, 0);
        mvwin(wnd, new_height, 0);
        
        wclear(wnd);
        setFrame();
    }

}
