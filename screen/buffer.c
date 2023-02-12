#include "private.h"

void clean_buffer(Buffer *buf)
{
    for (usize i = 0; i < buf->rows_len; i++)
    {
        for (usize j = 0; j < buf->lines_len; j++)
        {
            *unsafe_get_cell(buf, i, j) = ' ';
        }
    }
}

char *get_cell(Buffer *buf, usize x, usize y)
{
    if (x < buf->rows_len && y < buf->lines_len)
    {
        return unsafe_get_cell(buf, x, y);
    }

    return NULL;
}

// 以下､非公開関数

Buffer new_buffer(usize rows_len, usize lines_len)
{
    Buffer buf;

    if (rows_len == 0 || lines_len == 0)
    {
        buf.buf = NULL;
        buf.rows_len = 0;
        buf.lines_len = 0;
    }
    else
    {
        buf.buf = malloc(sizeof(char) * rows_len * lines_len);
        buf.rows_len = rows_len;
        buf.lines_len = lines_len;
        clean_buffer(&buf);
    }

    return buf;
}

void drop_buffer(Buffer *buf)
{
    if (buf->buf != NULL)
    {
        free(buf->buf);
        buf->buf = NULL;
    }

    buf->rows_len = 0;
    buf->lines_len = 0;
}

char *unsafe_get_cell(Buffer *buf, usize x, usize y)
{
    return &buf->buf[y * buf->rows_len + x];
}
