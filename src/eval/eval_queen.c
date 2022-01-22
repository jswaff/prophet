#include <prophet/parameters.h>

#include "eval_internal.h"

#include  <assert.h>

/**
 * \brief Evaluate a single queen.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the queen is on
 * \param endgame       if the eval should be done in the endgame phase 
 *
 * \return a score for the queen.
 */
int32_t eval_queen(const position_t* pos, square_t sq, bool UNUSED(endgame))
{
    assert(pos->piece[sq] == QUEEN || pos->piece[sq] == -QUEEN);

    int32_t score = 0; 

    if (is_white_piece(pos->piece[sq]))
    {
        score = queen_pst[sq];
    }
    else
    {
        score = queen_pst[flip_rank[sq]];
    }

    score += eval_major_on_7th(pos, sq);
  
    return score;    
}
