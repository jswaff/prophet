#include "output.h"

#include "prophet/move.h"

#include "string_utils.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

bool logging_enabled = false;

static pthread_mutex_t error_mutex;
static pthread_mutex_t output_mutex;
static FILE* logfile = 0;


/**
 * \brief Initialize logging.
 *
 */
void init_logging()
{
    if (logging_enabled && !logfile) {
        logfile = fopen("prophet.log", "w");
    }
}


/**
 * \brief Close the logfile.
 *
 */
void close_logfile() 
{
    if (logfile) {
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
void plog(const char* format, ...)
{
    pthread_mutex_lock(&output_mutex);

    va_list stdout_args;
    va_start(stdout_args, format);
    vfprintf(stdout, format, stdout_args);
    va_end(stdout_args);

    if (logging_enabled) {
        va_list log_args;
        va_start(log_args, format);
        vfprintf(logfile, format, log_args);
        va_end(log_args);
    }

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


void print_pv(move_line_t* pv, int32_t depth, int32_t score, uint64_t elapsed, uint64_t num_nodes)
{
    char* pv_buf = move_line_to_str(pv);
    uint64_t time_centis = elapsed / 10;
    plog("%2d %5d %5llu %7llu %s\n", depth, score, time_centis, num_nodes, pv_buf);
    free(pv_buf);
}
