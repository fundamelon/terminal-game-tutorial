#ifndef QUIZ_H
#define QUIZ_H



// minimum 16-bit unsigned int 2D vector
typedef struct {
    uint_fast16_t x;
    uint_fast16_t y;
} vec2ui;


// at least 16-bit int 2D vector
typedef struct {
    uint_fast16_t x;
    uint_fast16_t y;
} vec2i;


struct {
    vec2i pos;
    vec2i dir;
    char disp_char;
} player;



int init();
void run();

void skipMenu(bool);
void setDifficulty(int);
void setTime(int);

#endif
