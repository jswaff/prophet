#include <prophet/bitmap.h>
#include <prophet/parameters.h>

#include "eval_internal.h"

#include  <assert.h>

/**
 * \brief Evaluate a single bishop.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the bishop is on
 * \param endgame       if the eval should be done in the endgame phase 
 *
 * \return a score for the bishop.
 */
int32_t eval_bishop(const position_t* pos, square_t sq, bool UNUSED(endgame))
{
    assert(pos->piece[sq] == BISHOP || pos->piece[sq] == -BISHOP);

    int32_t score = 0; 

    if (is_white_piece(pos->piece[sq]))
    {
        score = bishop_pst[sq];
    }
    else
    {
        score = bishop_pst[flip_rank[sq]];
    }

  
    return score;    
}


/**
 * \brief Evaluate the position for bishop pairs.
 *
 * \param pos           a pointer to a chess position
 *
 * \return a score for the bishop pair.
 */
int32_t eval_bishop_pair(const position_t* pos)
{
    return (popcnt(pos->white_bishops) > 1 ? bishop_pair : 0) -
        (popcnt(pos->black_bishops) > 1 ? bishop_pair : 0);
}