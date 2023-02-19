#include "public.h"

i8 title(Screen *scr);

int main(void)
{
    Screen scr = new_screen(80, 24);

    cbreak();
    timeout(0);
    keypad(stdscr, TRUE);

    while (1)
    {
        switch (title(&scr))
        {
        case 0:
            game_init(&scr);
            break;
        case 1:
            drop_screen(&scr);
            return 0;
            break;
        }
    }
}

i8 title(Screen *scr)
{
    Window *logo = add_window(scr, 70, 13);
    memcpy(
        logo->buf.buf,
        "                                                                               #                                          #                         ##                              #        #  #   #     #        #######     ########                  ######    ########     #              #     #      #       #          #    #                 #              #     #      #     # #  #      #    #    ##### ####    #              #     #      #     #    #     #     #    #   # #  #    #             #      #      #         ##          #     ##### ####    #            ##      #      #         #          ##     ##### ####    #     #     ##       ########       ##          ##      #   #    #    ##   #    ##         #      #      ##         ##        #   ##   #     ####                                                   # ####  ##                                                                                  ",
        910);
    move_window(logo, scr->rows_len + 1, 0);

    struct timespec wait_time;
    wait_time.tv_sec = 0;
    wait_time.tv_nsec = 1.0 / (logo->x - (scr->rows_len - logo->buf.rows_len) / 2) * 1000000000.0;
    while (logo->x != (scr->rows_len - logo->buf.rows_len) / 2)
    {
        struct timespec start;
        timespec_get(&start, TIME_UTC);

        logo->x -= 1;
        draw_screen(scr);

        struct timespec stop;
        timespec_get(&stop, TIME_UTC);
        stop = sub_time(&stop, &start);
        stop = sub_time(&wait_time, &stop);
        nanosleep(&stop, NULL);
    }

    Window *menu = add_window(scr, 7, 2);
    memcpy(get_cell(&menu->buf, 2, 0), "start", 5);
    memcpy(get_cell(&menu->buf, 2, 1), "quit", 4);
    move_window(menu, (scr->rows_len - menu->buf.rows_len) / 2, (scr->lines_len + logo->buf.lines_len - menu->buf.lines_len) / 2);

    i8 cursor_pos = 0;
    timeout(-1);
    while (1)
    {
        char *cursor = get_cell(&menu->buf, 0, cursor_pos);
        *cursor = '>';
        draw_screen(scr);

        int key = getch();

        switch (key)
        {
        case '\n':
            goto end;
            break;
        case KEY_UP:
            cursor_pos--;
            break;
        case KEY_DOWN:
            cursor_pos++;
            break;
        }

        if (cursor_pos > 1)
            cursor_pos = 0;
        if (cursor_pos < 0)
            cursor_pos = 1;

        *cursor = ' ';
    }

end:
    remove_window(scr, menu);
    remove_window(scr, logo);
    timeout(0);
    return cursor_pos;
}
