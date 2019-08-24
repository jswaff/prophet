#include <stdint.h>
#include <sys/timeb.h>
#include <time.h>

/**
 * \brief Return the number of milliseconds since epoch.
 *
 * \return the number of milliseconds since epoch
 */
int32_t milli_timer()
{
    /* TODO: ftime is obsolete. */
    struct timeb t;
    ftime(&t);
    return (t.time * 1000) + t.millitm;
}
