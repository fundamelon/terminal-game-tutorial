# PART 4: Multiple windows

In case you missed it, the previous tutorial can be found [here](../part3).


---
### 4.1 Multi-Windowing

This is where ncurses gets tricky.
This section will focus largely on refactoring our old code to use the window system.

However, it's not all boring - we will also make the player a ship, add animations, a health bar, and a starry background!

When your game has separate "areas", such as a score display and the main game area, it's a good idea to update and operate on them separately.
That way, if your move the player, the entire screen won't need updating - just the game area.

Lucky for us, ncurses has a clever way of doing just that!

At the top of your `game.cpp`, replace your globals with:

```c++
/** includes **/

WINDOW* main_wnd;
WINDOW* game_wnd;

rect game_area;
rect screen_area;

vec2ui cur_size;

ObjectField asteroids;
ObjectField stars;

struct {
    vec2i pos;
    rect bounds;
    char disp_char;
    int energy;
} player;
```

This will set the stage for what we'll add later.

From now on, we will assume the game size will always be the default terminal size - 80x24, or 80 characters by 24 rows.

Let's begin by rewriting `init()`:

```c++
int init() {

    srand(time(0));

    main_wnd = initscr();
    cbreak();
    noecho();
    clear();
    refresh();

    curs_set(0);

    start_color();

    screen_area = { { 0, 0 }, { 80, 24 } };

/** see next snippet **/
```

The only new lines here are the random seeding at the top and defining screen area at the bottom.
80x24 is the default size of any terminal.
Note that our coordinates are still in the `(x, y` format here.

Moving on, we add:

```c++
/** see previous snippet **/

    int infopanel_height = 4;
    game_wnd = newwin(  screen_area.height() - infopanel_height - 2, 
                        screen_area.width() - 2, 
                        screen_area.top() + 1, 
                        screen_area.left() + 1  );
    main_wnd = newwin(screen_area.height(), screen_area.width(), 0, 0);

    game_area = { { 0, 0 }, { screen_area.width() - 2, screen_area.height() - infopanel_height - 4 } };

/** see next snippet **/
```

Here we are defining the bounds of our two windows using the function `newwin`.
[(man page)](http://linux.die.net/man/3/newwin)

The bounds are defined by simple counting, and as you can see, `game_wnd` excludes the borders that will be around the screen area (hence the `- 2`).

```c++
/** see previous snippet **/
   
    // useful color pairs
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
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

    // test for color here

    return 0;
}
```

Here, we simply define several useful color pairs, and setup the `keypad` and `nodelay` functions as before.
However, they need to be called for both windows now.

At this point, we find that our program needs a total overhaul.  
Therefore, we must rewrite our `run()` function, using the new two-window framework:

```c++
void run() {

    int tick;

    // initialize player as before

    // constrain object fields to game area
    asteroids.setBounds(game_area);
    stars.setBounds(game_area);

    int in_char = 0;
    bool exit_requested = false;
    bool game_over = false;
    
/** see next snippet **/
```

This is not much different from our original code, but helps in the refactoring process.

```c++
/** see previous snippet **/
    
    // draw frame around whole screen
    wattron(main_wnd, A_BOLD);
    box(main_wnd, 0, 0);
    wattroff(main_wnd, A_BOLD);

    // draw dividing line between game and stats
    wmove(main_wnd, game_area.bot() + 3, 1);
    whline(main_wnd, '-', screen_area.width() - 2);

    // initial draw
    wrefresh(main_wnd);
    wrefresh(game_wnd);

/** see next snippet **/
```

Here, we finally draw the outlines of the windows we defined earlier!
The screen now consists of a frame around the edge, and a horizontal dividing line near the bottom.
This divides our screen into the game area and the score area.

Continuing into the main game loop:
```c++
/** see previous snippet **/

    tick = 0;
    while(1) {

        // clear game window
        werase(game_wnd);

        // read inputs, lowercase all characters
        in_char = wgetch(main_wnd);
        in_char = tolower(in_char);

        // controls switch statement goes here
    
/** see next snippet **/
```

Now, we take advantage of the multi-windowing setup, and are now able to clear the game screen every execution.
This prevents us from having to "white-out" the trail behind every moving object!
Also, since the frame and score have been drawn previously, they will remain persistent.

```c++
/** see previous snippet **/

        // player ship main body
        wattron(game_wnd, A_BOLD);
        mvwaddch(game_wnd, player.pos.y, player.pos.x, player.disp_char);
        wattroff(game_wnd, A_BOLD);

        wrefresh(main_wnd);
        wrefresh(game_wnd);

        if(exit_requested || game_over) break;

        tick++;

        usleep(10000); // 10 ms
    };
}
```

Here we close up our game loop with drawing the character, refreshing each window separately, and performing our tick + sleep step.

