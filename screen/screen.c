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

    usize raw_scr_rows_len;
    usize raw_scr_lines_len;
    getmaxyx(stdscr, raw_scr_lines_len, raw_scr_rows_len);

    usize scr_x = 0;
    if (scr->rows_len < raw_scr_rows_len)
    {
        scr_x = (raw_scr_rows_len - scr->rows_len) / 2;
    }

    usize scr_y = 0;
    if (scr->lines_len < raw_scr_lines_len)
    {
        scr_y = (raw_scr_lines_len - scr->lines_len) / 2;
    }

    for (Window *win = scr->wins.first; win != NULL; win = win->next)
    {
        usize win_start_x = 0;
        if (win->x < 0)
        {
            win_start_x = -win->x;
        }

        usize win_start_y = 0;
        if (win->y < 0)
        {
            win_start_y = -win->y;
        }

        for (usize y = win_start_y; y < win->buf.lines_len && win->y + y < scr->lines_len && scr_y + win->y + y < raw_scr_lines_len; y++)
        {
            move(scr_y + win->y + y, scr_x + win->x + win_start_x);
            for (usize x = win_start_x; x < win->buf.rows_len && win->x + x < scr->rows_len && scr_x + win->x + x < raw_scr_rows_len; x++)
            {
                addch(*unsafe_get_cell(&win->buf, x, y));
            }
        }
    }
    refresh();
}
