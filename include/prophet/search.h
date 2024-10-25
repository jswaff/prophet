#pragma once

#include <prophet/position/position.h>

#include <stdint.h>

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus


/**
 * \brief Options structure for the search iterator.
 */
typedef struct 
{
    /**
     * \brief Is an early exit OK or should the search use all available time
     * or exhaust the maximum depth?
     */
    bool early_exit_ok;

    /**
     * \brief Maximum search depth
     */
    uint32_t max_depth;


    /**
     * \brief Maximum search time in milliseconds
     */
    uint32_t max_time_ms;


    /**
     * \brief If in post mode the iterator will print the best line to stdout
     * after each iteration, and a summary at the end.
     */
    bool post_mode;


    /**
     * \brief Clear hash before each search.  This is a debugging tool.
     */
    bool clear_hash_each_search;


} iterator_options_t;


/**
 * \brief Context structure for the search iterator.
 */
typedef struct 
{
    /**
     * \brief Pointer to a chess position to search
     */
    position_t* pos;

    /**
     * \brief Pre-allocated stack for move generation in the search.
     */
    move_t* move_stack;


    /**
     * \brief Pre-allocated stack for undo information.
     */
    undo_t* undo_stack;

} iterator_context_t;


/**
 * \brief Callback function when the PV is updated.
 */
typedef void (*pv_func_t)(move_line_t*, int32_t, int32_t, uint64_t, uint64_t);


/**
 * \brief Structure for tracking various stats during the search.
 */
typedef struct 
{

    /**
     * \brief The total number of nodes encountered in the search.
     */
    uint64_t nodes;

    /**
     * \brief The total number of qnodes encountered in the search.
     */
    uint64_t qnodes;

    /**
     * \brief The total number of exits due to a fail high (score exceeding 
     * beta) during the search.
     */
    uint64_t fail_highs;

    /**
     * \brief The total number of times the search failed to raise alpha.
     */
    uint64_t fail_lows;

    /**
     * \brief The total number of search exits due to a draw condition.
     */
    uint64_t draws;

    /**
     * \brief The number of fail highs from a hash hit
     */
    uint64_t hash_fail_highs;

    /**
     * \brief The number of fail lows from a hash hit
     */
    uint64_t hash_fail_lows;

    /**
     * \brief The number of exact scores from a hash hit
     */
    uint64_t hash_exact_scores;

} stats_t;


/**
 * \brief Structure for search optional data.
 */
typedef struct 
{
    /*
     * \brief A callback function to execute when the PV is updated at the root
     * of the search.
     */
    pv_func_t pv_callback;
    

    /*
     * \brief The time the search was started - should be milliseconds since
     * epoch.  This is used to calculate elapsed time when the callback is
     * executed.
     */
    uint64_t start_time;


    /*
     * \brief The time the search should stop, if non-zero.  Should be 
     * millseconds since epoch.
     */
    uint64_t stop_time;


    /*
     * \brief The number of nodes to wait between time checks.  This, along 
     * with node_count_last_time_check are used to avoid calling the expensive
     * time check routine at every node.
     */
    uint64_t nodes_between_time_checks;


    /*
     * \brief The node count at the last time check.  
     */
    uint64_t node_count_last_time_check;



} search_options_t;


/**
 * \brief Search the position using iterative deepening. 
 * 
 * \param opts          the options structure
 * \param ctx           the context for this search iterator
 *
 * \return the principal variation
 */ 
move_line_t iterate(const iterator_options_t* opts, 
    const iterator_context_t* ctx);


/**
 * \brief Search the position to until is is "quiet".
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
 * \param pos           the chess position
 * \param mv            the chess move to score
 * 
 * \return the score
 */
int32_t see(const position_t* pos, move_t mv);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus
