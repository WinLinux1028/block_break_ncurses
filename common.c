#include "public.h"

struct timespec add_time(struct timespec *x, struct timespec *y)
{
    struct timespec result;
    result.tv_sec = x->tv_sec + y->tv_sec;
    result.tv_nsec = x->tv_nsec + y->tv_nsec;

    if (result.tv_nsec >= 1000000000)
    {
        result.tv_sec += 1;
        result.tv_nsec -= 1000000000;
    }

    return result;
}

struct timespec sub_time(struct timespec *x, struct timespec *y)
{
    struct timespec result;
    result.tv_sec = x->tv_sec - y->tv_sec;
    result.tv_nsec = x->tv_nsec - y->tv_nsec;

    if (result.tv_nsec < 0)
    {
        result.tv_sec -= 1;
        result.tv_nsec += 1000000000;
    }

    if (result.tv_sec < 0)
    {
        result.tv_sec = 0;
        result.tv_nsec = 0;
    }

    return result;
}
