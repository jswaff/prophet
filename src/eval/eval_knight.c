#include "eval_internal.h"

#include  <assert.h>

/**
 * \brief Evaluate a single knight.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the knight is on
 *
 * \return a score for the knight.
 */
int32_t eval_knight(const position_t* pos, square_t sq)
{
    assert(pos->piece[sq] == KNIGHT || pos->piece[sq] == -KNIGHT);

    int32_t score = 0; 

    if (is_white_piece(pos->piece[sq]))
    {
        score = knight_pst[sq];
        score += knight_tropism * distance(sq, pos->black_king);
    }
    else
    {
        score = knight_pst[flip_rank[sq]];
        score += knight_tropism * distance(sq, pos->white_king);
    }

  
    return score;    
}
