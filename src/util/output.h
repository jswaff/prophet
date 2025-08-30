#pragma once

#include "prophet/move.h"

#include <stdint.h>
#include <stdio.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


/**
 * \brief Initialize logging.
 *
 */
void init_logging();


/**
 * \brief Close the logfile.
 *
 */
void close_logfile();


/**
 * \brief Write an error message to stderr.
 *
 * \param format        formatted string, followed by variable length
 *                      list of arguments.
 */
void error(const char* format, ...);


/**
 * \brief Write a message to stdout and the logfile.
 *
 * \param format        formatted string, followed by variable length
 *                      list of arguments.
 */
void plog(const char* format, ...);


/**
 * \brief Write a message to a stream.
 *
 * \param stream        the stream to write to
 * \param format        formatted string, followed by variable length
 *                      list of arguments.
 */
void out(FILE* stream, const char* format, ...);


void print_pv(move_line_t* pv, int32_t depth, int32_t score, uint64_t elapsed, uint64_t num_nodes);
void print_pv2(move_t* pv, int num_pv, int32_t depth, int32_t score, uint64_t elapsed, uint64_t num_nodes);

/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  
