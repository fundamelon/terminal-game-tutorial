# PART 3

In case you missed it, the previous tutorial can be found [here](../part2).


## Spawning an Object 
The next step in our game will be to create an object that will be used as an enemy. 
First, we will create a struct for our enemy. This struct will contain the x and y 
position of our enemy. We also want to create multiple enemies so we will make a vector
of enemy.
```c++
struct enemy{
    int x; // stores x position
    int y; // stores y position
};

vector<enemy> enemies;
```
We want to set the x position of our enemy randomly and our y position right below our window
frame. To make our x position random we will be using the `srand` `rand` and `time` 
These will require the `cstdlib.h` and `time.h` libraries.
```c++
#include "cstdlib.h"
#include "time.h"
// only libraries specific to this functionality are included 


bool init{
    srand(time(NULL); 
    // code omitted  
}

void spawnEnemy(){
     int pos = rand() % game_area.width(); // randomize enemy x position spawn
     enemy e;
     e.x = pos;
     e.y = 0; 
     enemies.push_back(e); // we will store our enemy in our vector
}
```
Now we will spawn our enemies on the window by using their x and y position and calling
our `mvaddch` function. 
