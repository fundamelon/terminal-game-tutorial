# PART 2

Click [here](../part1) in case you missed part 1.

### 2.1: Moving things around a little
To prepare for the rest of the project, we will now prototype some function headers for our game source. 

In a new file called ```src/game.h```, we add:
```c++
int init();
void run();

void skipMenu(bool);
void setDifficulty(int);
void setTime(int);
```
Along with standard compilation checks.  (Add these on your own.)
The latter three functions are "setters".  
They modify game variables, so that a caller does not need to have the variables themselves.
This is important for encapsulation in object-oriented programming, but it serves another purpose for us.

Using getter and setter functions in the source header allows us to modify globals that are only defined within the source file itself!
Although globals are usually avoided, they are convenient enough in this case to be worth taking advantage of.
This will prevent a **lot** of headaches later on.


Now, we will put things in their proper place - in a new file called ```src/game.cpp```:
```c++
#include <string>
#include <ncurses.h>

#include "game.h"

WINDOW* wnd;

int init() {
    wnd = initscr();
    cbreak();
    noecho();
    clear();
    refresh();

    return 0;
}


void run() {

    move(5, 5);

    std::string text = "Hello world!";
    for(int i = 0; i < text.size(); i++) {
        addch(text[i]);
        addch(' ');
    }

    refresh;

    while(1);
}
```
Note that this is just code copy-pasted from our original main function.  
We go back there and replace it with:
```c++
    int init_status = init();

    if(init_status != 0)
        run();

    return 0;
```
We can also remove all includes, and add ```#include "game.h"```.  

Finally, add a makefile target for game.cpp. 
The project should now behave the same as our Hello World program from last section.
There are several things to note:

<!-- NOTE: sentences need to be on one line in a bullet list -->

- First off, we are structuring the main and quiz code in a way that makes sense to both object-oriented or functional approaches.  If you wish, you can make game.h a class template, and convert everything to object-oriented format. The function ```init()``` can therefore be easily rewritten as a constructor, and vice versa.

- It is important to note the role of main.cpp.  Its purpose will be to parse command line input and pass the data along to the game, which will adjust accordingly.  This compartmentalizes code that would be very cluttered otherwise.  However, you are still welcome to use any approach you are comfortable with.

- Note also that we avoid using any special types in game.h, to avoid having redundant includes in .cpp files that don't need them.  The same goes for variables and structs - never declare them in your header!

### 2.2: Getting started for real

Let's finish up our initialization procedure.

Add the following snippets to ```init()```, in ```game.cpp```:
```c++
    /** clear(), refresh() **/

    keypad(wnd, true);
```

This option enables ncurses to interpret action keys, rather than print out escape sequences.
([man page](http://linux.die.net/man/3/keypad))

```c++
    nodelay(wnd, true);
```

This disables blocking when using [```wgetch()```](http://linux.die.net/man/3/wgetch).
It's important if we want to animate something while still listening for input.
([man page](http://linux.die.net/man/3/nodelay))


Next, we will set up color manipulation.

```c++
    if(!has_colors()) {
        endwin();
        printf("ERROR: Terminal does not support color.\n");
        exit(1);
    }
```

The function ```has_colors()``` helps us test whether or not the terminal supports color manipulation.
([man page](http://linux.die.net/man/3/has_colors))

And finally,
```c++
    start_color();
```

Enables routines that let you redefine colors within a terminal.
([man page](http://linux.die.net/man/3/start_color))


We'll come back to this later.
For now, let's return to the header prototype some more.

At the top of your game.h:
```c++
typedef struct {
    uint_fast8_t x;
    uint_fast8_t y;
} vec2ui;

typedef struct {
    int_fast8_t x;
    int_fast8_t y;
} vec2i;

/** init, run, etc **/
```
Here we declare a ```vec2ui``` datatype.
2D vectors will be the foundation of our game, and we won't be using floats.

The type ```uint_fast8_t``` is a c++11 feature* -
basically, it asks the compiler to implement that value using the *fastest* available ```int``` size of at *least* 8 bits.
For a better explanation, see [here](http://stackoverflow.com/questions/8500677/what-is-uint-fast32-t-and-why-should-it-be-used-instead-of-the-regular-int-and-u).  
<sub>* requires ```<cstdint>```</sub>


```vec2i``` is simply a signed version.
This means that the maximum range will be at least -127/128.  
This is OK, since our screen will be limited to 80x24.  (More on that in a bit!)

Now, we have everything we need to create our first character!

### 2.3: Player 1 has entered the game

A game isn't interactive if you can't control something.
Here, we will introduce our first movable element.

In the globals section of ```game.cpp```:
```c++
/** WINDOW wnd; **/

struct {
    vec2i pos;
    char disp_char;
} player;

/** int init() { **/
```

Here is our simple player.
All it owns is a position and a char to represent itself.

We'll go ahead and set those right now:
```c++
/** void run() { **/

    player.disp_char = '0';
    player.pos = {5, 10};

/** return 0; **/
```
You should also delete all the Hello World code in ```run()```, we won't need it anymore.
