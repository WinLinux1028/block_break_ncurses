#ifndef SCREEN_PRIVATE_H
#define SCREEN_PRIVATE_H

#include "public.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef WINDOWS
#include <ncursesw/ncurses.h>
#else
#include <ncurses.h>
#endif

extern Buffer new_buffer(usize rows_len, usize lines_len);
extern void drop_buffer(Buffer *buf);
extern char *unsafe_get_cell(Buffer *buf, usize x, usize y);

extern Window new_window(usize rows_len, usize lines_len);
extern void drop_window(Window *win);

extern WindowRoot new_window_root();
extern void drop_window_root(WindowRoot *wins);
extern Window *add_window_raw(WindowRoot *wins, usize rows_len, usize lines_len);
extern void remove_window_raw(WindowRoot *wins, Window *win);

#endif