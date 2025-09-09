#pragma once

#include <prophet/position.h>

#include <stdbool.h>
#include <stdint.h>

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus


/**
 * \brief Callback function when the PV is updated.
 */
typedef void (*pv_func_t)(move_t*, int, int32_t, int32_t, uint64_t, uint64_t);


/**
 * \brief Options structure for the search iterator.
 */
typedef struct {
    bool early_exit_ok;
    uint32_t max_depth;
    uint32_t max_time_ms;
    pv_func_t pv_callback;
    bool print_summary;
} iterator_options_t;


/**
 * \brief Context structure for the search iterator.
 */
typedef struct {
    position_t* pos;
    move_t* move_stack;
    undo_t* undo_stack;
} iterator_context_t;


/**
 * \brief Structure for tracking various stats during the search.
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
 * \brief Structure for search optional data.
 */
typedef struct {
    pv_func_t pv_callback;
    uint64_t start_time;
    uint64_t stop_time;
    uint64_t nodes_between_time_checks;
    uint64_t node_count_last_time_check;
} search_options_t;


/**
 * \brief Search the position using iterative deepening. 
 * 
 * \param depth         pointer to variable to record depth achieved by search
 * \param score         pointer to variable to record score returned by search
 * \param opts          the options structure
 * \param ctx           the context for this search iterator
 * \param stats         a stats structure to record the search statistics
 *
 * \return the principal variation
 */ 
move_line_t iterate(uint32_t* depth, int32_t* score, const iterator_options_t* opts, const iterator_context_t* ctx,
    stats_t *stats);

int iterate_from_fen(stats_t* stats, move_t* pv, int* pv_length, uint32_t* depth, int32_t* score, const char *fen,
    bool early_exit_ok, uint32_t max_depth, uint32_t max_time_ms, pv_func_t pv_callback);

int iterate_from_move_history(stats_t* stats, move_t* pv, int* pv_length, uint32_t* depth, int32_t* score,
    const move_t* move_history, int len_move_history, bool early_exit_ok, uint32_t max_depth, uint32_t max_time_ms,
    pv_func_t pv_callback);

/**
 * \brief Search the position to until it is "quiet".
 *
 * Quiescence search - attempt to obtain a score by searching until the 
 * position is quiet.
 *
 * \param pos           a pointer to a chess position
 * \param alpha         the lower bound
 * \param beta          the upper bound
 * \param move_stack    pre-allocated stack for move generation
 * \param undo_stack    pre-allocated stack for undo information
 * \param stats         structure for tracking search stats
 * \param opts          structure for tracking search options data
 * 
 * \return the score
 */
int32_t qsearch(position_t* pos, int32_t alpha, int32_t beta, 
    move_t* move_stack, undo_t* undo_stack, stats_t* stats, 
    search_options_t* opts);


/**
 * \brief Search the position to a fixed depth.
 *
 * \param pos           a pointer to a chess position
 * \param parent_pv     a pointer to the move line that will receive the PV
 * \param depth         the depth to search to
 * \param alpha         the lower bound
 * \param beta          the upper bound
 * \param move_stack    pre-allocated stack for move generation
 * \param undo_stack    pre-allocated stack for undo information
 * \param stats         structure for tracking search stats
 * \param opts          structure for tracking search options data
 * 
 * \return the score
 */
int32_t search(position_t* pos, move_line_t* parent_pv, int32_t depth, 
    int32_t alpha, int32_t beta, move_t* move_stack, undo_t* undo_stack,
    stats_t* stats, search_options_t* opts);


/**
 * \brief Score a move using the MVV/LVA algorithm.
 *
 * \param mv            the chess move to score
 * 
 * \return the score
 */
int32_t mvvlva(move_t mv);


/**
 * \brief Score a move using static exchange analysis (SEE)
 *
 * \param fen           the chess position
 * \param mv            the chess move to score
 * 
 * \return the score
 */
int32_t see_from_fen(const char *fen, move_t mv);


/**
 * \brief Set the search stop flag
 */
void set_search_stop_flag(bool stop);


/**
 * \brief Set the skip time checks flag
 */
void set_skip_time_checks_flag(bool skip);

// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus
