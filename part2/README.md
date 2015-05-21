# PART 2

Click [here](../part1) in case you missed part 1.

### 2.1: Moving things around a little
To prepare for the rest of the project, we will now prototype some function headers for our quiz source. 

In a file called src/quiz.h, we added:
```c++
int init();
int run();

void skipMenu(bool);
void setDifficulty(int);
void setTime(int);
void setQuestionSet(const char*);
```
along with standard compilation checks.  
The purposes of the latter four functions will be explained later - for now think of them as simple "setters".

Don't forget - it's necessary to ```include "quiz.h"``` in your main.cpp.

Now we will turn our project upside-down.  In a file called quiz.cpp, 
```c++
#include <string>
#include <ncurses.h>

#include "quiz.h"

WINDOW* wnd;

int init() {
    wnd = initscr();
    cbreak();
    noecho();
    clear();
    refresh();
}


int run() {

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
Speaking of which, we go back to there now and replace the entire thing with:
```c++
    init();
    run();
    return 0;
```
We can also remove all includes, save for ```#include "quiz.h"```.  

Finally, add a makefile target for quiz.cpp. 
The project should now behave the same as our Hello World program from last section.
Several things to note:

First off, we are structuring the main and quiz code in a way that makes sense to either object-oriented or functional approaches.  
If you wish, you can make quiz.h a class template, and convert everything to object-oriented format.
The function init() can therefore be easily made a constructor.

It is important to note the role of main.cpp.  
its purpose will be to parse command line input and pass the data along to the quiz, which will adjust accordingly.
This compartmentalizes code that would be very cluttered otherwise.  However, you are still welcome to use any approach you are comfortable with.

Note also that we avoid using any special types in quiz.h, to avoid having to add ```#include```s into the header itself.
