# PART 3: Falling objects

In case you missed it, the previous tutorial can be found [here](../part2).

##3.1 Intro
In Part 3, we will be going over how we can create objects for our game. These objects can be used for 
multiple things such as stars, asteroids, power-ups, etc.

## 3.2 Code Overview
Before we can make use of our objects, we must learn how our object classes work.  
#### 3.2.1 ObjectField.cpp
The ObjectField file contains our `ObjectField` and `SpaceObject` classes. `SpaceObject` holds the x and y position of
the object. `SpaceObject` is able to store and remove an object as well as update it's y position. 
The functions of `ObjectField` are provided below.
* `SpaceObject`
  * `SpaceObject()` intializes the x and y position of the object
  * `update()` increments the y position of the object
  * `getPos()` returns the position of the object

* `ObjectField`
  * `getData()` returns the vector containing our objects 
  * `erase(size_t i)` removes object from vector at index i 
  * `setBounds(rect a)` sets boundary of our objects to a
  * `update` erases objects that are out of bounds and calls `SpaceObject::update` to change the y position of the remaining objects.

## 3.3 Implementation
Our next step will be to implement the objects class in our `game.cpp` in order to make game elements. For our demo, we will
create stars that will scroll down our game window. (Remember to include appropriate libraries as well as code from 
previous parts or our next task may not work as desired).
#### 3.3.1 Initialization
In order to create these stars, we must declare our object and set it's boundaries. Since we have not yet defined a window
for our game, we will be using the max dimensions of the terminal for our boundary. The function `getmaxyx(int, int)` from 
the `ncurses` library will return these dimensions for us. We must then store these two values in a `rect` in order to pass 
it into `setBounds(rect a)`. 
```c++

ObjectField stars; // initialize our star object

void run(){
    
    // code omitted

    // these will be used to initialize rect
    uinti_fast16_t maxx, maxy;   

    // get window boundaries
    getmaxyx(wnd, maxy, maxx);

    //initialize our rect with 0 offset
    rect game_area = { {0, 0} , {maxx, maxy} }; 
    
    //set our star bounds
    stars.setBounds(game_area);

    while(1) {
        // code omitted   
    }
}

```
#### 3.3.2 Displaying Objects
Our next task is to display the objects we created in our game window. We can use the `mvaddch(wnd, int, int, char)` from the
`ncurses` library to do this. `mvaddch` displays a char at the corresponding position and of the provided window. Let's also
call `update` so our stars will scroll down the game window. So, we will update the object then remove it from it's old 
position and finally display it at it's new position.
```c++
while(1) {

    stars.update();

    // this removes each object from it's previous position on the screen
    // by placing it with  a space on the screen instead
    // this will be replaced by a more efficent method later
    for(auto s : stars.getData()) {
        mvaddch(s.getPos().y, s.getPos().x, ' ');
    }

    for(auto s : stars.getData()) {

        // use getPos() to get position of stars
        // displays given character at given position on game window
        mvaddch(s.getPos().y, s.getPos().x, '*');

    }

    usleep(10000); // wait for 1 ms

}
```
## 3.4 Conclusion
We have now completed our task, however there is still problems we need to fix. Our stars currently moved down the screen to
quickly. They also create strange results when they collide with the window frame or player. This fix will be discussed in 
our next section which can be found here [next section](../part4).
