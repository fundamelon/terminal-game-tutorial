# PART 1

### Get comfortable
This project will follow a standard structure which you should already be familiar with.
All source code belongs in ```src```, and all binary files are generated in ```bin```.
Let's start by creating a file, src/main.cpp:

```c++
#include <ncurses.h>

int main(int argv, char** argc) {

    return 0;
}
```

Note the inclusion of the header ```ncurses.h```.  
This is required to use the features we will rely on later.

Add a simple makefile:

```Makefile
bin/main: bin/main.o
        g++ -g -o bin/main bin/main.o -std=c++11 -Wall -pedantic -lncurses

bin/main.o:
        mkdir -p bin
        g++ -g -c -o bin/main.o src/main.cpp
```

This will become more complex with macros and such as our project grows, but for now it's enough.
Note the g++ ```-lncurses``` flag at the link stage - this links the proper library to our executable.

Try to ```make``` the project now. 
If you encounter an error related to the library, this probably means it's not installed.
You'll need to run ```sudo apt-get install libncurses5-dev```, or whichever is the latest version, to install it on your machine.  

*Note to ucr students on hammer: this will not compile with cs100 settings.  It works fine otherwise.*



This folder contains source code for part 1.
