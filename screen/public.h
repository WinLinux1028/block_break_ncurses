#ifndef SCREEN_PUBLIC_H
#define SCREEN_PUBLIC_H

#include <stdint.h>
typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
typedef intptr_t isize;
typedef uintptr_t usize;
typedef float f32;
typedef double f64;

typedef struct
{
    char *buf;
    usize rows_len;
    usize lines_len;
} Buffer;

extern void clean_buffer(Buffer *buf);
extern char *get_cell(Buffer *buf, usize x, usize y);

typedef struct Window
{
    Buffer buf;
    isize x;
    isize y;
    struct Window *next;
} Window;

extern void move_window(Window *win, isize x, isize y);

typedef struct
{
    Window *first;
} WindowRoot;

typedef struct
{
    WindowRoot wins;
    usize rows_len;
    usize lines_len;
} Screen;

extern Screen new_screen(usize rows_len, usize lines_len);
extern void drop_screen(Screen *scr);
extern Window *add_window(Screen *scr, usize rows_len, usize lines_len);
extern void remove_window(Screen *scr, Window *win);
extern void draw_screen(Screen *scr);

#endif
