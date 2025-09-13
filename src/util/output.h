#pragma once

#include "prophet/move.h"

#include <stdint.h>
#include <stdio.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


/**
 * @brief Initialize logging.
 */
void init_logging();


/**
 * @brief Close the logfile.
 */
void close_logfile();


/**
 * @brief Write an error message to stderr.
 *
 * @param format        formatted string, followed by variable length list of arguments
 */
void error(const char *format, ...);


/**
 * @brief Write a message to stdout and the logfile.
 *
 * @param format        formatted string, followed by variable length list of arguments
 */
void plog(const char *format, ...);


/**
 * @brief Write a message to a stream.
 *
 * @param stream        the stream to write to
 * @param format        formatted string, followed by variable length list of arguments.
 */
void out(FILE *stream, const char *format, ...);


/**
 * @brief Write a move line to stdout and the logfile.
 * 
 * @param pv            the move line
 * @param depth         the depth searched
 * @param score         the score
 * @param elapsed       the time elapsed, in ms
 * @param num_nodes     the number of nodes visited
 */
void print_pv(move_line_t *pv, int32_t depth, int32_t score, uint64_t elapsed, uint64_t num_nodes);


/**
 * @brief Write a move line to stdout and the logfile.
 * 
 * @param pv            a pointer to the start of an array of moves
 * @param num_pv        the number of moves to print
 * @param depth         the depth searched
 * @param final_for_depth if the PV is the final for this depth
 * @param score         the score
 * @param elapsed       the time elapsed, in ms
 * @param num_nodes     the number of nodes visited
 */
void print_pv2(move_t *pv, int num_pv, int32_t depth, bool final_for_depth, int32_t score, uint64_t elapsed, 
    uint64_t num_nodes);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  
