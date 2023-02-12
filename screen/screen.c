#include "private.h"

Screen new_screen(usize rows_len, usize lines_len)
{
    initscr();
    noecho();
    curs_set(0);

    Screen scr;
    scr.wins = new_window_root();
    scr.rows_len = rows_len;
    scr.lines_len = lines_len;

    return scr;
}

void drop_screen(Screen *scr)
{
    endwin();

    drop_window_root(&scr->wins);
    scr->rows_len = 0;
    scr->lines_len = 0;
}

Window *add_window(Screen *scr, usize rows_len, usize lines_len)
{
    return add_window_raw(&scr->wins, rows_len, lines_len);
}

void remove_window(Screen *scr, Window *win)
{
    return remove_window_raw(&scr->wins, win);
}

void draw_screen(Screen *scr)
{
    erase();
    for (Window *win = scr->wins.first; win != NULL; win = win->next)
    {

        usize start_y;
        if (win->y < 0)
        {
            start_y = win->y * (-1);
        }
        else
        {
            start_y = 0;
        }

        usize start_x;
        if (win->x < 0)
        {
            start_x = win->x * (-1);
        }
        else
        {
            start_x = 0;
        }

        for (usize j = start_y; j < win->buf.lines_len && j + win->y < scr->lines_len; j++)
        {
            move(win->y + j, win->x + start_x);
            for (usize k = start_x; k < win->buf.rows_len && k + win->x < scr->rows_len; k++)
            {
                addch(*unsafe_get_cell(&win->buf, k, j));
            }
        }
    }
    refresh();
}
