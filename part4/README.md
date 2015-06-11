# PART 4

In case you missed it, the previous tutorial can be found [here](../part3).


---
### 4.1 Multi-Windowing

This is where ncurses gets tricky.
This section will focus largely on refactoring our old code to use the window system.

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

```

The only new lines here are the random seeding at the top and setting screen area at the bottom.
The function `wresize` simply resizes a window with the given 
