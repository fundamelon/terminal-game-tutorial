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

std::vector<enemy> enemies;
std::vector<star> stars;

struct {
    vec2i pos;
    vec2i dir;
    rect bounds;
    char disp_char;
    char ship_type;
    bool moving;
    int energy;
} player;



int init() {
    
    srand(time(0));

    initscr();

    cbreak();
    noecho();
    clear();
    refresh();

    curs_set(0);

    start_color();

    // read in window size
    cur_size = { 0, 0 };
    getmaxyx(main_wnd, cur_size.x, cur_size.y);

    // define area for screen (default terminal size)
    screen_area = { { 0, 0 }, { 80, 24 } };

    // set screen size accordingly
    wresize(main_wnd, screen_area.height(), screen_area.width());

    // initialize window areas
    int infopanel_height = 4;
    game_wnd = newwin(screen_area.height() - infopanel_height - 2, screen_area.width() - 2, screen_area.top() + 1, screen_area.left() + 1);
    main_wnd = newwin(screen_area.height(), screen_area.width(), 0, 0);

    // define area for movement
    game_area = { { 0, 0}, { screen_area.width() - 2, screen_area.height() - infopanel_height - 5 } };

    applyColorscheme(COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);

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

    int tick = 0;
   
    // initialize player
    player.disp_char = 'o';
    player.pos = {10, 10};
    player.bounds = { { player.pos.x - 1, player.pos.y }, { 3, 2 } }; // player is 3 wide, 2 tall
    player.moving = false;
    player.energy = 100;

    int in_char;
    bool exit_requested = false;
    bool game_over = false;
   
    // draw frame around whole screen
    wattron(main_wnd, A_BOLD);
    box(main_wnd, 0, 0);
    wattroff(main_wnd, A_BOLD);

    // draw dividing line between game and stats
    wmove(main_wnd, game_area.bot() + 4, 1);
    whline(main_wnd, '-', screen_area.width() - 2);

    // initial draw
    wrefresh(main_wnd);
    wrefresh(game_wnd);

    while(1) {

        // clear game window
        werase(game_wnd);
 
        // TODO: Give warning message if screen is too small!
        if(cur_size.x > screen_area.width() || cur_size.y > screen_area.height()) {}
        //winResize(cur_width, cur_height);
    
        // read in input key, if any (non-blocking as defined earlier)
        in_char = wgetch(main_wnd);
        in_char = tolower(in_char);

        
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
                if(player.pos.y < game_area.bot() + 2)
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
                if(player.pos.x < game_area.right() - 2)
                    player.pos.x += 1; 
                break;
            default: 
                break;
        }


        if(tick % 10 == 0)
            moveStars();

        if(tick > 1000 && tick % 30 == 0)
            enemyAI();
    
        player.bounds = { { player.pos.x - 1, player.pos.y }, { 3, 2 } }; 

        // collision detection
        for(size_t i = 0; i < enemies.size(); i++) {
            if(player.bounds.contains(enemies.at(i).pos)) {
                enemies.erase(enemies.begin() + i);
                player.energy -= 10;
            }
        }

        if(player.energy <= 0)
            game_over = true;


        // draw starry background
        for(auto s : stars)
            mvwaddch(game_wnd, s.pos.y, s.pos.x, '.');  

        // player ship main body
        wattron(game_wnd, A_BOLD);
        mvwaddch(game_wnd, player.pos.y, player.pos.x, player.disp_char); // (y, x)
        wattroff(game_wnd, A_BOLD);

        // player ship accessories
        wattron(game_wnd, A_ALTCHARSET);
        //mvaddch(player.pos.y - 1, player.pos.x, ACS_UARROW);
        mvwaddch(game_wnd, player.pos.y, player.pos.x - 1, ACS_LARROW);
        mvwaddch(game_wnd, player.pos.y, player.pos.x + 1, ACS_RARROW);

        // animate engine flame :)
        if(tick / 5 % 3) { // 5 ms cycle, 50% duty
            wattron(game_wnd, COLOR_PAIR(tick % 2 ? 3 : 4));
            mvwaddch(game_wnd, player.pos.y + 1, player.pos.x, ACS_UARROW);
            wattroff(game_wnd, COLOR_PAIR(tick % 2 ? 3 : 4));
        }

        wattroff(game_wnd, A_ALTCHARSET);

       
        // draw enemies
        for(auto n : enemies) {
            wattron(game_wnd, A_BOLD);
            mvwaddch(game_wnd, n.pos.y, n.pos.x, '*');
            wattroff(game_wnd, A_BOLD);
        }


        // draw UI elements
        // energy bar
        wmove(main_wnd, 20, 1);
        whline(main_wnd, ' ', 25); // health bar is 25 chars long
        wmove(main_wnd, 20, 1);
        drawEnergyBar(player.energy);

        // draw static string to hold percentage
        mvwprintw(main_wnd, 21, 1, " - E N E R G Y -      //");

        // draw numeric percentage
        wattron(main_wnd, A_BOLD);
        if(player.energy <= 25) {
            wattron(main_wnd, COLOR_PAIR(4));
            if(tick % 100 < 50)
                mvwprintw(main_wnd, 21, 18, "%i%%", player.energy); 
            wattroff(main_wnd, COLOR_PAIR(4));
        } else
            mvwprintw(main_wnd, 21, 18, "%i%%", player.energy); 
        wattroff(main_wnd, A_BOLD);

        //usleep(100);

        // refresh windows
        wrefresh(main_wnd);
        wrefresh(game_wnd);

        if(exit_requested || game_over) break;

        tick++;

        //nanosleep({0, 1000000000}, NULL);
        usleep(10000); // 1 ms
    };

    delwin(main_wnd);
    delwin(game_wnd);

    endwin();

    if(game_over) printf("Game over!\n");
}



void applyColorscheme(short fg, short bg) {
    init_pair(1, fg, bg);
    wbkgd(main_wnd, COLOR_PAIR(1));
    wbkgd(game_wnd, COLOR_PAIR(1));
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

    for(size_t i = 0; i < enemies.size(); i++){ // move each enemy down
        if(enemies.at(i).pos.y > game_area.bot()){ // delete from vector when enemy reaches bottom
        //    mvwaddch(game_wnd, n.at(i).pos.y , n.at(i).pos.x, ' ');
            enemies.erase(enemies.begin() + i);
        }
        //mvwaddch(game_wnd, n.at(i).pos.y, n.at(i).pos.x, ' '); // remove enemy from prev pos
        enemies.at(i).pos.y += 1; // move enemy down 
        //mvwaddch(game_wnd, n.at(i).pos.y, n.at(i).pos.x, '*');
    }

    int pos = rand() % game_area.width(); // randomize enemy x position spawn 

    enemy e;
    e.pos.x = pos;
    e.pos.y = 0;
    enemies.push_back(e);
}



void moveStars() {

    for(size_t i = 0; i < stars.size(); i++) {
        if(stars.at(i).pos.y > game_area.bot())
            stars.erase(stars.begin() + i);

        stars.at(i).pos.y += 1;
    }

    int pos = rand() % game_area.width();

    star s;
    s.pos.x = pos;
    s.pos.y = 0;
    stars.push_back(s);  
}



void drawEnergyBar(int a) {

    int col_pair = 1;
    for(int i = 0; i < a; i+=4) {
        if(i > 100)
            col_pair = 5; // blue
        else if(i > 50)
            col_pair = 2; // green
        else if(i > 25)
            col_pair = 3; // yellow
        else
            col_pair = 4; // red

        wattron(main_wnd, COLOR_PAIR(col_pair));
        wattron(main_wnd, A_BOLD);
        waddch(main_wnd, '/');
        wattroff(main_wnd, A_BOLD);
        wattroff(main_wnd, COLOR_PAIR(col_pair));
    }
}

