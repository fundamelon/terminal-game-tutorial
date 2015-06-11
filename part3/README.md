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
our bounds as well as a vector of `SpaceObject` to store our objects. We must be able to set the
bounds of our object so we will create a void function with one parameter of `rect`
called `SetBounds`. We also need to be able to access our objects in our vector. We will do this 
by creating a function `getData()` which returns a `std::vector` of type `SpaceObject`. Our
next function `erase(size_t)` will be able to delete an object from our vector. Finally, we 
need an `update()` function that will move our object down the game window and delete them 
when they are out of bounds. Let's put this together. 
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
First, let's initialize our two objects asteroids and stars. We must first declare our objects
and then set their bounds. Finally, we call the update member of our object to spawn them.
```c++
ObjectField asteroids;
ObjectField stars;

void run(){
    
    // code omitted

    //set our bounds
    asteroids.setBounds(game_area);
    stars.setBounds(game_area);

    while(1) {
        stars.update();
        asteroids.update();
        
        // again not all code is present

        usleep(10000); // 1 ms
    }
}
```
Now our objects are spawning and moving down our game window. However, there is a problem we
must address. This problem is that our object will be spawing and scrolling down the window
at a very fast rate. To fix this, we take advantage of the function `usleep`. Our game has
usleep waiting 1 ms every time we go through our loop. We can make use of this by creating an
`int` variable `tick`. We will increment tick every 1 ms. Then we will have our function call
every so many ticks depending on the speed we want it to scroll down. Now we have a method
for calling our objects `update` member at an interval that is more convenient to us. For our
game we will call `update` on stars every 50 ms. We will also call `update` on asteroids every
20 ms after the 100 ms mark. Let's try it now.
```c++
ObjectField asteroids;
ObjectField stars;

void run(){
    
    int tick = 0;
    // code omitted

    //set our bounds
    asteroids.setBounds(game_area);
    stars.setBounds(game_area);

    while(1) {
        if(tick % 50 == 0)
            stars.update();
        
        if(tick > 100 && tick % 20 == 0)
            asteroids.update();
        
        // again not all code is present

        usleep(10000); // 1 ms
        tick++;
    }
}
```
## 3.4 Displaying Our Objects 
Now that we have our objects properly spawning and moving down the game window, let us 
actually make them appear on our window. We will do this by using the `mvwaddch` function
from the curses library. We will also be using `wattron` and `wattroff` to make our 
objects stand out more. Let's try this.
```c++
ObjectField asteroids;
ObjectField stars;

void run(){
    
    int tick = 0;
    // code omitted

    //set our bounds
    asteroids.setBounds(game_area);
    stars.setBounds(game_area);

    while(1) {
        if(tick % 50 == 0)
            stars.update();
        
        if(tick > 100 && tick % 20 == 0)
            asteroids.update();
        
        // again not all code is present

        for(auto s : stars.getData()) {
            // displays given character at given position on game window
            mvwaddch(game_wnd, o.getPos().y, o.getPos().x, '*');
        }

        // go through each object and make them appear on the window
        for(auto o : asteroids.getData()) {

            //makes our characters to display bold
            wattron(game_wnd, A_BOLD);
            
            // displays given character at given position on game window
            mvwaddch(game_wnd, o.getPos().y, o.getPos().x, '*');

            // turn off our BOLD effects 
            wattroff(game_wnd, A_BOLD);

        }

        usleep(10000); // 1 ms
        tick++;
    }
}
```
Now that you know how to create scrolling objects, you are ready to move on to the [next section](../part4).
