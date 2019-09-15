#include "eval_internal.h"

#include  <assert.h>

/**
 * \brief Evaluate a single queen.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the queen is on
 *
 * \return a score for the queen.
 */
int32_t eval_queen(const position_t* pos, square_t sq)
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
