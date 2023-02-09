#ifndef PUBLIC_H
#define PUBLIC_H

#include "screen/public.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#ifdef WINDOWS
#include <ncursesw/ncurses.h>
#else
#include <ncurses.h>
#endif

extern struct timespec add_time(struct timespec *x, struct timespec *y);
extern struct timespec sub_time(struct timespec *x, struct timespec *y);

extern void game(Screen *scr);

typedef struct
{
    Window *win;
    struct
    {
        isize x;
        isize y;
    } before_win_pos;
    struct
    {
        f64 x;
        f64 y;
    } pos;
    struct
    {
        f64 v;
        f64 theta;
    } speed;
} Ball;

extern Ball new_ball(Screen *scr);
extern void drop_ball(Screen *scr, Ball *ball);
extern void move_ball(Ball *ball, usize x, usize y);
extern void set_speed_ball(Ball *ball, f64 v, f64 theta);
extern void reverse_y_speed_ball(Ball *ball);
extern void reverse_x_speed_ball(Ball *ball);
extern void calc_ball(Ball *ball, f64 sec);

#endif