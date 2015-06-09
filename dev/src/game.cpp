#include <unistd.h>
#include <ncurses.h>

#include <cstdint>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "game.h"



WINDOW* main_wnd;
WINDOW* game_wnd;

rect game_area;
rect screen_area;

vec2ui cur_size;

std::vector<enemy> n;

struct {
    vec2i pos;
    vec2i dir;
    vec2ui bounds;
    char disp_char;
    char ship_type;
} player;



int init() {
    
    srand(time(0));

    initscr();

    cbreak();
    noecho();
    clear();
    refresh();


    start_color();

    setColorscheme(COLOR_WHITE, COLOR_BLACK);

    cur_size = { 0, 0 };
    getmaxyx(main_wnd, cur_size.x, cur_size.y);


    // define area for screen (default terminal size)
    screen_area = { { 0, 0 }, { 80, 24 } };

    // set screen size accordingly
    wresize(main_wnd, screen_area.height(), screen_area.width());

    int infopanel_height = 4;

    game_wnd = newwin(screen_area.height() - infopanel_height - 2, screen_area.width() - 2, screen_area.top() + 1, screen_area.left() + 1);
    main_wnd = newwin(screen_area.height(), screen_area.width(), 0, 0);

    // define area for movement
    game_area = { { 0, 0}, { screen_area.width() - 3, screen_area.height() - infopanel_height - 5 } };

    // enable function keys
    keypad(main_wnd, true);
    keypad(game_wnd, true);

    // disable input blocking
    nodelay(main_wnd, true);
    nodelay(game_wnd, true);

    // enable color
    if(!has_colors()) {
        endwin();
        printf("ERROR: Terminal does not support color.\n");
        exit(1);
    }

    return 0;
}



void run() {

    int wait = 10;
    
    player.disp_char = 'o';
    player.pos = {10, 10};
    player.bounds = { 3, 2 }; // player is 3 wide, 2 tall

    int in_char;
    bool exit_requested = false;
    curs_set(0);
   
    // draw frame around whole screen
    attron(A_BOLD);
    box(main_wnd, 0, 0);
    attroff(A_BOLD);

    // draw dividing line between game and stats
    wmove(main_wnd, game_area.bot() + 4, 1);
    whline(main_wnd, '-', screen_area.width() - 2);

    wrefresh(main_wnd);
    wrefresh(game_wnd);

    while(1) {

        werase(game_wnd);
        //move(0, 0);
        //for(int i = 0; i < 80 * 24; i++) addch(' ');
   
        // TODO: Give warning message if screen is too small!
        if(cur_size.x > screen_area.width() || cur_size.y > screen_area.height()) {}
        //winResize(cur_width, cur_height);
    
        in_char = wgetch(main_wnd);
        in_char = tolower(in_char);

        if(wait == 10){
            enemyAI();
            wait = 0;
        }

        switch(in_char) {
            case 'q': 
                exit_requested = true; 
                break;
            case KEY_UP:
            case 'w':
            case 'i':
                if(player.pos.y > game_area.top())
                    player.pos.y -= 1;
                break;
            case KEY_DOWN:
            case 's':
            case 'k':
                if(player.pos.y < game_area.bot() + 1)
                    player.pos.y += 1;
                break;
            case KEY_LEFT: 
            case 'a':
            case 'j':
                if(player.pos.x > game_area.left() + 1)
                    player.pos.x -= 1;
                break;
            case KEY_RIGHT: 
            case 'd':
            case 'l':
                if(player.pos.x < game_area.right() - 1)
                    player.pos.x += 1; 
                break;
            default: 
                break;
        }

        mvwaddch(game_wnd, player.pos.y, player.pos.x, player.disp_char); // (y, x)
        attron(A_ALTCHARSET);
        mvwaddch(game_wnd, player.pos.y + 1, player.pos.x, ACS_UARROW);
        //mvaddch(player.pos.y - 1, player.pos.x, ACS_UARROW);
        mvwaddch(game_wnd, player.pos.y, player.pos.x - 1, ACS_LARROW);
        mvwaddch(game_wnd, player.pos.y, player.pos.x + 1, ACS_RARROW);
        attroff(A_ALTCHARSET);

        usleep(100);

        wrefresh(main_wnd);
        wrefresh(game_wnd);

        if(exit_requested) break;

        //nanosleep({0, 1000000000}, NULL);
        usleep(10000); // 1 ms
        wait++;
    };

    delwin(main_wnd);
    delwin(game_wnd);

    endwin();
}



void setColorscheme(short fg, short bg) {
    init_pair(1, fg, bg);
    wbkgd(main_wnd, COLOR_PAIR(1));
}



void setFrame(){
    // creates simple frame around window composed of vertical and horizontal lines
    attron(A_BOLD);
    box(main_wnd, 0, 0);
    attroff(A_BOLD);
    
    // border characters can be set manually using the border function
    // border( wnd, leftside, rightside, topside, bottom side, tlcorner, 
    //                                      trcorner, blcorner, brcorner);
}



void winResize(int &orig_width, int &orig_height){
    int new_width, new_height;

    getmaxyx(main_wnd, new_width, new_height); 

    // if window dimensions have changed, update border
    if(new_width != orig_width || new_height != orig_height){
        orig_width = new_width;
        orig_height = new_height;
        
        wresize(main_wnd, new_height, 0);
        mvwin(main_wnd, new_height, 0);
        
        wclear(main_wnd);
        setFrame();
    }

}

void enemyAI(){
    int pos = rand() % 76 + 1; // randomize enemy x position spawn 

    enemy e;
    e.pos.x = pos;
    e.pos.y = 1;
    n.push_back(e);

    for(size_t i = 0; i < n.size(); i++){ // move each enemy down
        if(n.at(i).pos.y == 22){ // delete from vector when enemy reaches bottom
            mvwaddch(game_wnd, n.at(i).pos.y , n.at(i).pos.x, ' ');
            n.erase(n.begin() + i);
        }
        mvwaddch(game_wnd, n.at(i).pos.y, n.at(i).pos.x, ' '); // remove enemy from prev pos
        n.at(i).pos.y += 1; // move enemy down 
        mvwaddch(game_wnd, n.at(i).pos.y, n.at(i).pos.x, '*');
    }
}
