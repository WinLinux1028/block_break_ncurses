#include "public.h"
#define PADDLE_LENGTH 10
#define BOUNCE_ANGLE_MIN 7.0
#define BOUNCE_ANGLE_MAX 11.0

typedef struct
{
    Window *player;
    struct
    {
        Window *win;
        usize remain;
    } blocks;
    Ball ball;
} GameSession;

u8 execute(GameSession *session, Screen *scr);
u8 collision_detection(GameSession *session, Screen *scr);
void game_event(GameSession *session, Screen *scr, u8 state);

void game(Screen *scr)
{
    srand((unsigned int)time(NULL));

    f64 ball_speed = 10;
    while (1)
    {
        GameSession session;
        session.player = add_window(scr, PADDLE_LENGTH, 1);
        for (usize i = 0; i < PADDLE_LENGTH; i++)
        {
            *get_cell(&session.player->buf, i, 0) = '#';
        }
        move_window(session.player, (scr->rows_len - session.player->buf.rows_len) / 2, scr->lines_len - 1);

        session.blocks.win = add_window(scr, scr->rows_len * 0.9, 4);
        move_window(session.blocks.win, (scr->rows_len - session.blocks.win->buf.rows_len) / 2, 2);
        session.blocks.remain = 0;
        for (usize i = 0; i < session.blocks.win->buf.rows_len; i++)
        {
            for (usize j = 0; j < session.blocks.win->buf.lines_len; j++)
            {
                if (i % 2 == 0)
                {
                    *get_cell(&session.blocks.win->buf, i, j) = '#';
                    session.blocks.remain += 1;
                }
            }
        }

        session.ball = new_ball(scr);
        move_ball(&session.ball, (scr->rows_len - session.ball.win->buf.rows_len) / 2, session.player->y - 1);
        set_speed_ball(&session.ball, ball_speed, (BOUNCE_ANGLE_MIN + (BOUNCE_ANGLE_MAX - BOUNCE_ANGLE_MIN) * (double)rand() / RAND_MAX) * M_PI / 6.0);

        struct timespec wait_time;
        wait_time.tv_sec = 0;
        wait_time.tv_nsec = 16666667;
        while (1)
        {
            struct timespec start;
            timespec_get(&start, TIME_UTC);

            u8 state = execute(&session, scr);
            if (state != 0)
            {
                remove_window(scr, session.player);
                remove_window(scr, session.blocks.win);
                drop_ball(scr, &session.ball);

                if (state == 1)
                {
                    return;
                }
                else if (state == 2)
                {
                    break;
                }
            }
            draw_screen(scr);

            struct timespec stop;
            timespec_get(&stop, TIME_UTC);
            stop = sub_time(&stop, &start);
            stop = sub_time(&wait_time, &stop);
            nanosleep(&stop, NULL);
        }

        ball_speed += 1;
    }
}

u8 execute(GameSession *session, Screen *scr)
{
    int key = getch();
    switch (key)
    {
    case 27:
        return 1;
        break;
    case KEY_LEFT:
        session->player->x -= 1;
        break;
    case KEY_RIGHT:
        session->player->x += 1;
        break;
    }

    session->ball.before_win_pos.x = session->ball.win->x;
    session->ball.before_win_pos.y = session->ball.win->y;
    calc_ball(&session->ball, 0.01666666666666666644);

    u8 state = collision_detection(session, scr);
    if (state != 0)
    {
        game_event(session, scr, state);
        return state;
    }

    return 0;
}

u8 collision_detection(GameSession *session, Screen *scr)
{
    if (session->player->x < 0)
    {
        session->player->x = 0;
    }
    else if (session->player->x > scr->rows_len - session->player->buf.rows_len)
    {
        session->player->x = scr->rows_len - session->player->buf.rows_len;
    }

    if (session->ball.pos.x < 0)
    {
        session->ball.win->x = 0;
        session->ball.pos.x = session->ball.win->x;
        reverse_x_speed_ball(&session->ball);
    }
    else if (session->ball.pos.x > scr->rows_len - session->ball.win->buf.rows_len)
    {
        session->ball.win->x = scr->rows_len - session->ball.win->buf.rows_len;
        session->ball.pos.x = session->ball.win->x;
        reverse_x_speed_ball(&session->ball);
    }

    if (session->blocks.win->x <= session->ball.win->x && session->ball.win->x < session->blocks.win->x + session->blocks.win->buf.rows_len && session->blocks.win->y <= session->ball.win->y && session->ball.win->y < session->blocks.win->y + session->blocks.win->buf.lines_len)
    {
        char *cell = get_cell(&session->blocks.win->buf, session->ball.win->x - session->blocks.win->x, session->ball.win->y - session->blocks.win->y);
        if (*cell == '#')
        {
            *cell = ' ';
            session->blocks.remain -= 1;

            if (session->blocks.remain == 0)
            {
                return 2;
            }

            if (session->ball.before_win_pos.x == session->ball.win->x)
            {
                reverse_y_speed_ball(&session->ball);
            }
            else if (session->ball.before_win_pos.y == session->ball.win->y)
            {
                reverse_x_speed_ball(&session->ball);
            }
            else
            {
                reverse_y_speed_ball(&session->ball);
                reverse_x_speed_ball(&session->ball);
            }
        }
    }

    if (session->ball.pos.y < 0)
    {
        session->ball.win->y = 0;
        session->ball.pos.y = session->ball.win->y;
        reverse_y_speed_ball(&session->ball);
    }
    else if (session->ball.pos.y > scr->lines_len - session->ball.win->buf.lines_len)
    {
        if (session->player->x <= session->ball.pos.x && session->ball.pos.x <= session->player->x + session->player->buf.rows_len - 1)
        {
            session->ball.win->y = scr->lines_len - session->ball.win->buf.lines_len;
            session->ball.pos.y = session->ball.win->y;

            if (session->ball.pos.x < session->player->x + ((f64)session->player->buf.rows_len - 1) / 2)
            {
                f64 max = (BOUNCE_ANGLE_MAX + BOUNCE_ANGLE_MIN) / 2 - 1;
                session->ball.speed.theta = (BOUNCE_ANGLE_MIN + (max - BOUNCE_ANGLE_MIN) * (double)rand() / RAND_MAX) * M_PI / 6.0;
            }
            else
            {
                f64 min = (BOUNCE_ANGLE_MAX + BOUNCE_ANGLE_MIN) / 2 + 1;
                session->ball.speed.theta = (min + (BOUNCE_ANGLE_MAX - min) * (double)rand() / RAND_MAX) * M_PI / 6.0;
            }
        }
        else
        {
            return 1;
        }
    }

    return 0;
}

void game_event(GameSession *session, Screen *scr, u8 state)
{
    timeout(-1);
    Window *art_1 = NULL;
    Window *art_2 = NULL;
    if (state == 1)
    {
        art_1 = add_window(scr, 65, 6);
        memcpy(art_1->buf.buf,
               "                                                                                                                                    ####     #    ##    ##  #####      ####   #     # #####  ####   ##   #    #    ##    ##  #         ##   #   #   #  #      #   #  #        # #   ###  # #  #         #     #  #   #  #      #   #  #        # #   # #  # #  #####     #     #  #   #  #####  #   # ",
               390);
        art_2 = add_window(scr, 65, 6);
        memcpy(art_2->buf.buf,
               " #   ### #   #  # #  # #  #         #     #   # #   #      ####   #     # #####  #  ##  #  #         #     #   # #   #      #  ##  ##   ## #   #  #  ##  #  #         ##   #    ##    #      #   #   ##### #     # #      #  #####      ####      #    #####  #    #                                                                                                                                  ",
               390);
    }
    if (state == 2)
    {
        art_1 = add_window(scr, 73, 6);
        memcpy(art_1->buf.buf,
               "                                                                                                                                                    ### #######   #     ####   #####      ####   #     #####    #    ####   #       #      #    ##   #  #         ##   #  #     #        #    #   #  #       #     # #   #       #         #       #     #       # #   #   #  ###     #     # #   #       #####     #       #     #####   # #   #   # ",
               438);
        art_2 = add_window(scr, 73, 6);
        memcpy(art_2->buf.buf,
               "   ##    #    #   #  #   ### #         #       #     #      #   #  ####       #   #    #####  #     # #         #       #     #      #####  #  ##  #   #   #    #   #  ##   ## #         ##   #  #     #      #   #  #   #  ####    #   #     #  #####  #####      ####   ##### ##### #     # #    #                                                                                                                                                  ",
               438);
    }
    move_window(art_1, scr->rows_len + 1, 0);
    move_window(art_2, -art_2->buf.rows_len - 1, art_1->y + art_1->buf.lines_len);

    struct timespec wait_time;
    wait_time.tv_sec = 0;
    wait_time.tv_nsec = 1.0 / (art_1->x - (scr->rows_len - art_1->buf.rows_len) / 2) * 1000000000.0;
    while (1)
    {
        struct timespec start;
        timespec_get(&start, TIME_UTC);

        u8 flag1 = art_1->x != (scr->rows_len - art_1->buf.rows_len) / 2;
        u8 flag2 = art_2->x != (scr->rows_len - art_2->buf.rows_len) / 2;
        if (!(flag1 || flag2))
        {
            break;
        }
        if (flag1)
        {
            art_1->x -= 1;
        }
        if (flag2)
        {
            art_2->x += 1;
        }
        draw_screen(scr);

        struct timespec stop;
        timespec_get(&stop, TIME_UTC);
        stop = sub_time(&stop, &start);
        stop = sub_time(&wait_time, &stop);
        nanosleep(&stop, NULL);
    }

    Window *msg = add_window(scr, 24, 1);
    memcpy(msg->buf.buf, "Press enter to continue.", 24);
    move_window(msg, (scr->rows_len - msg->buf.rows_len) / 2, (scr->lines_len + art_1->buf.lines_len + art_2->buf.lines_len - msg->buf.lines_len) / 2);
    draw_screen(scr);
    while (1)
    {
        if (getch() == '\n')
        {
            break;
        }
    }

    remove_window(scr, art_1);
    remove_window(scr, art_2);
    remove_window(scr, msg);
    timeout(0);
}