#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>


static pthread_mutex_t error_mutex;
static pthread_mutex_t output_mutex;
static FILE* logfile = 0;


/**
 * \brief Initialize logging.
 *
 */
void init_logging()
{
    if (!logfile)
    {
        logfile = fopen("prophet.log", "w");
    }
}


/**
 * \brief Close the logfile.
 *
 */
void close_logfile() 
{
    if (logfile)
    {
        fclose(logfile);
    }
}


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
 * \brief Write a message to stdout and the logfile.
 *
 * \param format        formatted string, followed by variable length
 *                      list of arguments.
 */
void logout(const char* format, ...)
{
    pthread_mutex_lock(&output_mutex);

    va_list stdout_args;
    va_start(stdout_args, format);
    vfprintf(stdout, format, stdout_args);
    va_end(stdout_args);

    va_list log_args;
    va_start(log_args, format);
    vfprintf(logfile, format, log_args);
    va_end(log_args);

    pthread_mutex_unlock(&output_mutex);
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
