# PART 3

In case you missed it, the previous tutorial can be found [here](../part2).


## 3.1 Creating Our Object 

The next step in our game will be to create an object class that will be used to store objects
such as asteroids, stars, power ups, etc. To do this, we will create two different classes in
our `ObjectField.h` file.

#### 3.1.1 Making our SpaceObject Class
Our SpaceObject class will be the used for the object itself. The object will consist of
an x and y position in our game window. To keep track of this, we will do this by using our
`vec2i` struct from earlier. Remember `vec2i` holds two integers, one will be for our
x position and one for our y position. We can now declare a constructor for our class. This will look like `SpaceObject(int, int)`. Now let's create some functions that will allow us to 
manipulate our object. Our first function will be able to set the position of our object.
Let's call this function `setPos`. It will return a `void` and have a single parameter 
of `vec2i`. Now that we can set our position, we will create a function that will return the
position of our object. Our final function for this class will update the y position of our
object in order to scroll our object down the screen. We will call it `update()` and it will 
return a `void` and take no parameters. Now, let's see what this will look like.
```c++
class SpaceObject {

public: 
    SpaceObject(int, int); // constructor
    void update(); // updates our y value 

    vec2i getPos() const; // gets the position of our object
    void setPos(vec2i); // sets the position of our object

private:
    vec2i pos; // holds position of our object

};
```
#### 3.1.2 Making Our ObjectField Class
Our ObjectField class will be used to store and delete our objects as well as set the 
boundaries that our object will be in. Our class will consist of a `rect` type which will define
our bounds. The type `rect` we defined consists of a `vec2i` for our offset and another `vec2i`
for our boundary. Our class also contains a vector of `SpaceObject` to store our objects. We
must be able to set the bounds of our object so we will create a void function with one parameter
of `rect `called `SetBounds`. We also need to be able to access our objects in our vector. We
will do this by creating a function `getData()` which returns a `std::vector` of type
`SpaceObject`. Our next function `erase(size_t)` will be able to delete an object from our
vector. Finally,we need an `update()` function that will move our object down the game window
and delete them when they are out of bounds. Let's put this together. 
```c++
class ObjectField {

public:
    void update();
    void erase(size_t);
    std::vector<SpaceObject> getData() const;

    void setBounds(rect);

private:
    rect field_bounds;
    std::vector<SpaceObject> object_set;

};
```
## 3.2 Implementing our Object Functions
Our next step will be to actually implement the functions in our object classes. 
First let's implement the very simple functions that are self-explanatory. 
```c++ 
void ObjectField::erase(size_t i) {
    // this will erase an object at position i in our vector
    object_set.erase(object_set.begin() + i); 
}

// returns vector holding our objects
std::vector<Space_Object> ObjectField::getData() const { return object_set; }

// sets the boundaries of our objects
void ObjectField::setBounds(rect a) { field_bounds = a; }

// Object Constructor    
SpaceObject::SpaceObject(int nx, int ny) { pos.x = nx; pos.y = ny; }

// increments the y position of our object so it will scroll down our game window
void SpaceObject::update() { pos.y += 1; }

// returns the position of our object
vec2i SpaceObject::getPos() const { return pos; }
```
Next, let's look at our final and most complicated function we need to implement. Our 
`ObjectField::update()` function must spawn a new object on the game window. It also updates
the position of each and every object in our vector. Finally, it must check if our object is
within the game boundaries and delete it from the vector if it is not. This must be done so
our vector does not got too big and take up all of our memory. Let's try and implement it.
```c++
void ObjectField::update() {
    for(size_t i = 0; i < object_set.size(); i++){
        //checks if object is out of bounds
        if(object_set.at(i).getPos().y > field_bounds.bot())
            object_set.erase(object_set.begin() + i);

        // moves our object down the game window
        object_set.at(i).update(); 
    }

    // spawn a new object at a random x pos and at the top of the screen
    SpaceObject s( rand() % field_bounds.width(), 0);
    object_set.push_back(s);
}
```
## 3.3 Making Use of Our Objects
Now that we can create objects that move down our game window we can add many features to our
game. We can use this to create asteroids that can attack our player or even stars that work
as a background to our game.
First, let's initialize our stars. We must first declare our objectand then set it's bounds.
For now we will set its bounds by getting the max size of the window with `getmaxyx` and using 
the x and y values for our `rect` value. Later we will learn how to properly set these
boundaries. For now we must deal with the bugs created by not having proper boundaries such as 
our window frame disappearing. Finally, we call the update member of our object to spawn them.
```c++
ObjectField stars;

void run(){
    
    // code omitted

    // these will be used to initialize rect
    uinti_fast16_t maxx, maxy;   

    // get window boundaries
    getmaxyx(wnd, maxy, maxx);

    //initialize our rect with 0 offset
    rect game_area = { {0, 0} , {maxx, maxy} };
    
    //set our bounds
    stars.setBounds(game_area);

    while(1) {
        stars.update();
        
        // again not all code is present

        usleep(10000); // 1 ms
    }
}
```
Now our objects are spawning and moving down our game window. However, there is a problem we
must address. The problem is that our object will be spawing and scrolling down the window
at a very fast rate. To fix this, we take advantage of the function `usleep`. Our game has
usleep waiting 1 ms every time we go through our loop. We can make use of this by creating an
`int` variable `tick`. We will increment tick every 1 ms. Then we will have our function call
every so many ticks depending on the speed we want it to scroll down. Now we have a method
for calling our objects member function `update` at an interval that is more convenient to us.
For our game we will call `update` on stars every 50 ms. Let's try it now.
```c++
ObjectField stars;

void run(){
    
    int tick = 0;

    // code omitted

    // these will be used to initialize rect
    uinti_fast16_t maxx, maxy;   

    // get window boundaries
    getmaxyx(wnd, maxy, maxx);

    //initialize our rect with 0 offset
    rect game_area = { {0, 0} , {maxx, maxy} };

    //set our bounds
    stars.setBounds(game_area);

    while(1) {
        if(tick % 50 == 0)
            stars.update();
        
        // again not all code is present

        usleep(10000); // 1 ms
        tick++;
    }
}
```
## 3.4 Displaying Our Objects 
Now that we have our objects properly spawning and moving down the game window, let us 
actually make them appear on our window. We will do this by using the `mvaddch` function
from the curses library. Since we have not learned a better way to clear the screen yet, 
we must replace the old object with a space to clear from the screen. Let's try this.
```c++
ObjectField stars;

void run(){
    
    int tick = 0;
    // code omitted

    // these will be used to initialize rect
    uinti_fast16_t maxx, maxy;   

    // get window boundaries
    getmaxyx(wnd, maxy, maxx);

    //initialize our rect with 0 offset
    rect game_area = { {0, 0} , {maxx, maxy} };


    //set our bounds
    stars.setBounds(game_area);

    while(1) {
        if(tick % 50 == 0)
            stars.update();
        
        // this removes each object from it's previous position on the screen
        for(auto s : stars.getData()) {
            mvaddch(s.getPos().y, s.getPos().x, ' ');
        }

        // again not all code is present

        for(auto s : stars.getData()) {
            // displays given character at given position on game window
            mvaddch(s.getPos().y, s.getPos().x, '*');
        }

        usleep(10000); // 1 ms
        tick++;
    }
}
```
Now that you know how to create scrolling objects, you are ready to move on to the [next section](../part4).
