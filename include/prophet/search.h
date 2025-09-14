#pragma once

#include <prophet/move.h>

#include <stdbool.h>
#include <stdint.h>

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus


/**
 * @brief Callback function when the PV is updated.
 */
typedef void (*pv_func_t)(move_t*, int, int32_t, bool, int32_t, uint64_t, uint64_t);


/**
 * @brief Structure for tracking various stats during the search.
 */
typedef struct {
    uint64_t nodes;
    uint64_t qnodes;
    uint64_t fail_highs;
    uint64_t fail_lows;
    uint64_t hash_fail_highs;
    uint64_t hash_fail_lows;
    uint64_t hash_exact_scores;
    uint64_t draws;
} stats_t;


/**
 * @brief Search the position using iterative deepening. 
 * 
 * @param stats         a stats structure to record the search statistics
 * @param pv            a structure to record the principal variation
 * @param pv_length     a pointer to an integer to record the length of the principal variation
 * @param depth         pointer to variable to record depth achieved by search
 * @param score         pointer to variable to record score returned by search
 * @param fen           the chess position to search
 * @param early_exit_ok whether it is OK for the search to terminate early
 * @param max_depth     the maximum depth to search
 * @param max_time_ms   the maximum time in milliseconds to search
 * @param pv_callback   a callback function to invoke when the principal variation is updated
 *
 * @return 0 on success, non-zero on failure
 */ 
int iterate_from_fen(stats_t *stats, move_t *pv, int *pv_length, uint32_t *depth, int32_t *score, const char *fen,
    bool early_exit_ok, uint32_t max_depth, uint32_t max_time_ms, pv_func_t pv_callback);


/**
 * @brief Search the position using iterative deepening. 
 * 
 * @param stats         a stats structure to record the search statistics
 * @param pv            a structure to record the principal variation
 * @param pv_length     a pointer to an integer to record the length of the principal variation
 * @param depth         pointer to variable to record depth achieved by search
 * @param score         pointer to variable to record score returned by search
 * @param move_history  the sequence of moves to make from the initial position to arrive at the search position
 * @param len_move_history the number of moves in move_history
 * @param early_exit_ok whether it is OK for the search to terminate early
 * @param max_depth     the maximum depth to search
 * @param max_time_ms   the maximum time in milliseconds to search
 * @param pv_callback   a callback function to invoke when the principal variation is updated
 *
 * @return 0 on success, non-zero on failure
 */ 
int iterate_from_move_history(stats_t *stats, move_t *pv, int *pv_length, uint32_t *depth, int32_t *score,
    const move_t *move_history, int len_move_history, bool early_exit_ok, uint32_t max_depth, uint32_t max_time_ms,
    pv_func_t pv_callback);


/**
 * @brief Score a move using static exchange analysis (SEE).
 *
 * @param fen           the chess position
 * @param mv            the chess move to score
 * 
 * @return the score
 */
int32_t see_from_fen(const char *fen, move_t mv);


/**
 * @brief Set the search stop flag.
 */
void set_search_stop_flag(bool stop);


/**
 * @brief Set the skip time checks flag.
 */
void set_skip_time_checks_flag(bool skip);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus
