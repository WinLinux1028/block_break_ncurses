#include "private.h"

void move_window(Window *win, isize x, isize y)
{
    win->x = x;
    win->y = y;
}

// 以下､非公開関数

Window new_window(usize rows_len, usize lines_len)
{
    Window win;
    win.buf = new_buffer(rows_len, lines_len);
    win.x = 0;
    win.y = 0;
    win.next = NULL;
    return win;
}

void drop_window(Window *win)
{
    drop_buffer(&win->buf);
    win->x = 0;
    win->y = 0;
}

WindowRoot new_window_root()
{
    WindowRoot wins;
    wins.first = NULL;

    return wins;
}

void drop_window_root(WindowRoot *wins)
{
    Window *win = wins->first;
    while (win != NULL)
    {
        drop_window(win);
        Window *next = win->next;
        free(win);
        win = next;
    }
    wins->first = NULL;
}

Window *add_window_raw(WindowRoot *wins, usize rows_len, usize lines_len)
{
    Window *win;
    if (wins->first == NULL)
    {
        wins->first = malloc(sizeof(Window));
        win = wins->first;
    }
    else
    {
        for (Window *i = wins->first; i != NULL; i = i->next)
        {
            win = i;
        }
        win->next = malloc(sizeof(Window));
        win = win->next;
    }

    *win = new_window(rows_len, lines_len);
    return win;
}

void remove_window_raw(WindowRoot *wins, Window *win)
{
    drop_window(win);

    Window *before = NULL;
    for (Window *i = wins->first; i != NULL; i = i->next)
    {
        if (i == win)
        {
            if (before == NULL)
            {
                wins->first = i->next;
            }
            else
            {
                before->next = i->next;
            }
            free(i);
            break;
        }
        before = i;
    }
}
