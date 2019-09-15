#include "eval_internal.h"

#include  <assert.h>


/**
 * \brief Evaluate a single rook.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the rook is on
 *
 * \return a score for the rook.
 */
int32_t eval_rook(const position_t* pos, square_t sq)
{
    assert(pos->piece[sq] == ROOK || pos->piece[sq] == -ROOK);

    int32_t score = 0; 

    if (is_white_piece(pos->piece[sq]))
    {
        score = rook_pst[sq];
    }
    else
    {
        score = rook_pst[flip_rank[sq]];
    }

    score += eval_major_on_7th(pos, sq);
  
    return score;    
}
