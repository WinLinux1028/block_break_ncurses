#include "public.h"

Ball new_ball(Screen *scr)
{
    Ball ball;
    ball.win = add_window(scr, 1, 1);
    *get_cell(&ball.win->buf, 0, 0) = 'o';

    ball.before_win_pos.x = 0;
    ball.before_win_pos.y = 0;

    ball.pos.x = 0;
    ball.pos.y = 0;

    ball.speed.v = 0;
    ball.speed.theta = 0;

    return ball;
}

void drop_ball(Screen *scr, Ball *ball)
{
    remove_window(scr, ball->win);
    ball->pos.x = 0;
    ball->pos.y = 0;
    ball->speed.v = 0;
    ball->speed.theta = 0;
}

void move_ball(Ball *ball, usize x, usize y)
{
    move_window(ball->win, x, y);
    ball->pos.x = x;
    ball->pos.y = y;
}

void set_speed_ball(Ball *ball, f64 v, f64 theta)
{
    ball->speed.v = v;
    ball->speed.theta = theta;
}

void reverse_y_speed_ball(Ball *ball)
{
    ball->speed.theta = 2 * M_PI - ball->speed.theta;
}

void reverse_x_speed_ball(Ball *ball)
{
    reverse_y_speed_ball(ball);
    if (ball->speed.theta < M_PI)
    {
        ball->speed.theta += M_PI;
    }
    else
    {
        ball->speed.theta -= M_PI;
    }
}

void calc_ball(Ball *ball, f64 sec)
{
    ball->pos.x += ball->speed.v * cos(ball->speed.theta) * sec;
    ball->pos.y += ball->speed.v * sin(ball->speed.theta) * sec;
    ball->win->x = round(ball->pos.x);
    ball->win->y = round(ball->pos.y);
}
