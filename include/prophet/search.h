#ifndef _SEARCH_H_
#define _SEARCH_H_

#include <prophet/position/position.h>
#include <stdint.h>


// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus


typedef struct 
{
    uint64_t nodes;
    uint64_t fail_highs;
} stats_t;


/**
 * \brief Search the position to a fixed depth.
 *
 * \param pos           a pointer to a chess position
 * \param depth         the depth to search to
 * \param alpha         the lower bound
 * \param beta          the upper bound
 * \param move_stack    pre-allocated stack for move generation
 * \param stats         structure for tracking search stats
 * 
 * \return the score
 */
int32_t search(position_t* pos, int32_t depth, int32_t alpha, int32_t beta, 
    move_t* move_stack, stats_t* stats);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _SEARCH_H_ */
