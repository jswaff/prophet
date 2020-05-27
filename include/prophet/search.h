#ifndef _SEARCH_H_
#define _SEARCH_H_

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
    int max_depth;

    /**
     * \brief If in post mode the iterator will print the best line to stdout
     * after each iteration, and a summary at the end.
     */
    bool post_mode;    

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
typedef void (*pv_func_t)(move_line_t*);


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

} stats_t;


/**
 * \brief Search the position using iterative deepening. 
 * 
 * \param opts          the options structure
 * \pram ctx            the context for this search iterator
 *
 * \return the principal variation
 */ 
move_line_t iterate(const iterator_options_t* opts, 
    const iterator_context_t* ctx);


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
 * \param pv_callback   callback when the PV is updated at the root
 * 
 * \return the score
 */
int32_t search(position_t* pos, move_line_t* parent_pv, int32_t depth, 
    int32_t alpha, int32_t beta, move_t* move_stack, undo_t* undo_stack,
    stats_t* stats, pv_func_t pv_callback);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _SEARCH_H_ */
