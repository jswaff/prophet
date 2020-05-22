#include <stdint.h>
#include <time.h>

/**
 * \brief Return the number of milliseconds since epoch.
 *
 * \return the number of milliseconds since epoch
 */
uint64_t milli_timer()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);


    return (uint64_t)ts.tv_sec * 1000 + (uint64_t)(ts.tv_nsec / 1000000UL);
}
