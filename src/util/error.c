#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

static pthread_mutex_t logfile_mutex;

/**
 * \brief Write an error message to stderr.
 *
 */
void error(const char* format, ...)
{
    pthread_mutex_lock(&logfile_mutex);
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    pthread_mutex_unlock(&logfile_mutex);
}

