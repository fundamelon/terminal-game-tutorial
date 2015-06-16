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
Our next step will be to implement the objects class to make game elements. For our demo, we will create stars that will 
scroll down our game window. 
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
#### 3.3.3 Slowing the Scrolling
As you can see from the code above, our stars are falling down the window too fast. The fix for this is simple. We will
create an `int` variable named tick. Tick will be incremented every time we go through our loop. This will allow us to 
call `stars.update()` after ever x ticks. Now we will not be moving our stars down the window as quickly. For our purpose,
we will call `update` after every 50 ms (50 ticks). Let's take a look at what our code will look like. 
```c++
int tick = 0;

while(1) {
    
    if(tick % 50 == 0) // call every 50 ticks
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
    tick++;

}

```

## 3.4 Conclusion
Now that you have learned how to use objects to create game elements, you are ready to move on to the [next section](../part4).
