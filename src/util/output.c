#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

static pthread_mutex_t error_mutex;
static pthread_mutex_t output_mutex;


/**
 * \brief Write an error message to stderr.
 *
 * \param format        formatted string, followed by variable length
 *                      list of arguments.
 */
void error(const char* format, ...)
{
    pthread_mutex_lock(&error_mutex);
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    pthread_mutex_unlock(&error_mutex);
}


/**
 * \brief Write a message to a stream.
 *
 * \param stream        the stream to write to
 * \param format        formatted string, followed by variable length
 *                      list of arguments.
 */
void out(FILE* stream, const char* format, ...)
{
    pthread_mutex_lock(&output_mutex);
    va_list args;
    va_start(args, format);
    vfprintf(stream, format, args);
    va_end(args);
    pthread_mutex_unlock(&output_mutex);
}
