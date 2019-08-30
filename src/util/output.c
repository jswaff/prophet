#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

static pthread_mutex_t error_mutex;
static pthread_mutex_t output_mutex;


/**
 * \brief Write an error message to stderr.
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
 * \brief Write a error message to stdout.
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
