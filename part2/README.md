# PART 2

Click [here](../part1) in case you missed part 1.

### 2.1: Moving things around a little
To prepare for the rest of the project, we will now prototype some function headers for our game source. 

In a file called src/game.h, we added:
```c++
int init();
void run();

void skipMenu(bool);
void setDifficulty(int);
void setTime(int);
```
along with standard compilation checks.  (Add these on your own.)

The purposes of the latter three functions will be explained later - for now think of them as simple "setters".

Now we will put things in their proper place.  
In a file called game.cpp, 
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

# (NOTE: sentences need to be on one line in a bullet list)

- First off, we are structuring the main and quiz code in a way that makes sense to both object-oriented or functional approaches.  If you wish, you can make game.h a class template, and convert everything to object-oriented format. The function ```init()``` can therefore be easily rewritten as a constructor, and vice versa.

- It is important to note the role of main.cpp.  Its purpose will be to parse command line input and pass the data along to the game, which will adjust accordingly.  This compartmentalizes code that would be very cluttered otherwise.  However, you are still welcome to use any approach you are comfortable with.

- Note also that we avoid using any special types in game.h, to avoid having redundant includes in .cpp files that don't need them.  The same goes for variables and structs - never declare them in your header!

### 2.2: Shooting for a working program

