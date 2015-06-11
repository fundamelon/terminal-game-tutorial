#ifndef QUIZ_H
#define QUIZ_H



// minimum 16-bit unsigned int 2D vector
typedef struct {
    uint_fast8_t x;
    uint_fast8_t y;
} vec2ui;


// at least 16-bit int 2D vector
typedef struct {
    int_fast8_t x;
    int_fast8_t y;
} vec2i;


int init();
void run();

void skipMenu(bool);
void setDifficulty(int);
void setTime(int);

void setColorscheme(short fg, short bg);
void setFrame();

#endif
