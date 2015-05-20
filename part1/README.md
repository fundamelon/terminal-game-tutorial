# PART 1

### Getting comfortable
This project will follow the standard structure which you should already be familiar with.
All source code belongs in ```src```, and all binary files are generated in ```bin```.
Let's start by creating a file called src/main.cpp:

```c++
#include <iostream>
#include <ncurses.h>

int main(int argv, char** argc) {

    return 0;
}
```

And a simple makefile:

```Makefile
bin/main: bin/main.o
        g++ -g -o bin/main bin/main.o -std=c++11 -Wall -pedantic -lncurses

bin/main.o:
        mkdir -p bin
        g++ -g -c -o bin/main.o src/main.cpp
```

This folder contains source code for part 1.
