# PART 3

In case you missed it, the previous tutorial can be found [here](../part2).


## 3.1 Creating Our Enemies
The next step in our game will be to create an object that will be used as an enemy. 
First, we will create a struct for our enemy. This struct will contain the x and y 
position of our enemy which we can keep track of using our `vec2i` struct from earlier.
```c++
struct enemy{
    vec2i pos;
};
```
## 3.2 Initializing Our Enemy Position
We want to set the x position of our enemy randomly and our y position to always begin 
right below our window frame.
To do this we will first learn how to get a random number for our given range which will allow us 
to initialize our x position.
#### 3.2.1 Randomizing Enemy x Position
To make our x position random we will be using the the `cstdlib.h` and `time.h` libraries 
Now that we have included our libraries we can use the `srand` `rand` and `time` functions to 
get a random number.
To do this we must first initialize `srand` using our `time` function in our initializer function.
This guarantees that every call to rand will return a pseudo random value.
Next we want to manipulate `rand` so that we get a random value in our desired range.
All that we need to do is mod `rand` return value by the upper bound of the range we want. In our case this
will we be the width of our game window.
Let's take a look at what this would look like.
```c++
#include "cstdlib.h"
#include "time.h"

int init() { // srand must be called once in our initializer function only

    srand(time(NULL); 

    // code omitted  

}

void EnemyAI() {

     // this returns a random value between 0-game_area.width() - 1
     int pos = rand() % game_area.width(); // randomize enemy x position spawn
     
     // code omitted

}
```
## 3.3 Storing Our Enemy 
Now that we have learned how to initialize our enemy position we can move on to our next step. Since
we want to have more than one enemy, we must store them all in one set. To do this we will be using
a `std::vector` of type `enemy`. This is necessary because when we start moving our enemies down the screen,
we must be able to access all of them easily. The process is simple, we create an enemy, initialize, and 
store it in our enemy vector. Let's see what it looks like.
```c++
struct enemy {
    int2i pos;
};

vector<enemy> enemies; // stores our enemies

void run() {
    
    // code omitted

    spawnEnemy();

}

void EnemyAI(){

    int pos = rand() % game_area.width();

    enemy e;

    e.pos.x = pos;
    e.pos.y = 0;

    enemies.push_back(e);

}
```
##3.4 Spawning Our Enemy
Now that we know how to make our enemies and store them properly, we want to be able to display them 
on game window. This step will require us to use the `mvwaddch` function. We will also be using 
`wattron` to make our enemies more visible in the game. Let's take a look at how this would look.
```c++
struct enemy {
    int2i pos;
};

vector<enemy> enemies; // stores our enemies

void run() {
    
    // code omitted

    spawnEnemy();

    for(auto n : enemies) {

        // this make our enemy BOLD and stand out more
        wattron(game_wnd, A_BOLD); 

        // this displays the enemy '*' on the game window at the corresponding position
        mvwaddch(game_wnd, n.pos.y, n.pos.x, '*'); 

        // we want to turn this effect off for the rest of objects on the game window
        wattroff(game_wnd, A_BOLD);

    }

}

void EnemyAI(){

    int pos = rand() % game_area.width();

    enemy e;

    e.pos.x = pos;
    e.pos.y = 0;

    enemies.push_back(e);

}

```
## 3.5 Moving Our Enemy 
So now we have our enemies on the screen, however they just appear at the top of our game window.
Our final step will be making our enemies scroll down the screen. 
To do this, we will be incrementing the y position of each enemy in our vector. Each enemy will 
continue to move down until it reaches the bottom of our game boundary. At this point we will
want to delete the enemy from our vector so our vector does not get ridiculously big and eat
up all of our memory. Let's implement it now.
```c++
struct enemy {
    int2i pos;
};

vector<enemy> enemies; // stores our enemies

void run() {
    
    // code omitted

    spawnEnemy();
    for(auto n : enemies) {

        // this make our enemy BOLD and stand out more
        wattron(game_wnd, A_BOLD); 

        // this displays the enemy '*' on the game window at the corresponding position
        mvwaddch(game_wnd, n.pos.y, n.pos.x, '*'); 

        // we want to turn this effect off for the rest of objects on the game window
        wattroff(game_wnd, A_BOLD);

    }

}

void EnemyAI(){

    int pos = rand() % game_area.width();

    for(size_t i = 0; i < enemies.size(); i++) {

        if(enemies.at(i).pos.y > game_area.bot()){ // delete enemy from vector when out of bounds
            enemies.erase(enemies.begin() + i);
        }

        enemies.at(i).pos.y += 1; // move enemy down

    }

    enemy e;

    e.pos.x = pos;
    e.pos.y = 0;

    enemies.push_back(e);

}
```
