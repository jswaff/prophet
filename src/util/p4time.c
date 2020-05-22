#include <stdint.h>
#include <time.h>

/**
 * \brief Return the number of milliseconds since epoch.
 *
 * \return the number of milliseconds since epoch
 */
int32_t milli_timer()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);


    return ts.tv_sec * 1000 + (ts.tv_nsec / 1000000UL);
}
