#include "eval_internal.h"

#include  <assert.h>

/**
 * \brief Evaluate a single king.
 * 
 * \param pos           a pointer to a chess position
 * \param sq            the square the king is on
 * \param mgscore       a pointer to the middle game score accumulator
 * \param egscore       a pointer to the endgame score accumulator
 *
 */
void eval_king(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore)
{
    assert(pos->piece[sq] == KING || pos->piece[sq] == -KING);
    assert(sq == pos->white_king || sq == pos->black_king);

    if (is_white_piece(pos->piece[sq])) {
        *mgscore += king_pst_mg[sq];
        *mgscore += eval_king_safety(pos, true);
        *egscore += king_pst_eg[sq];
    } else {
        int32_t flipsq = flip_rank[sq];
        *mgscore -= king_pst_mg[flipsq];
        *mgscore -= eval_king_safety(pos, false);
        *egscore -= king_pst_eg[flipsq];
    }

}
