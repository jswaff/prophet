#include "time.h"

#include <stdint.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif


uint64_t milli_timer()
{
#ifdef _WIN32
    return (uint64_t)GetTickCount64();
#else
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    return (uint64_t)ts.tv_sec * 1000 + (uint64_t)(ts.tv_nsec / 1000000UL);
#endif
}
