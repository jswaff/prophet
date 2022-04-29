#include <prophet/parameters.h>

#include "eval_internal.h"

#include  <assert.h>

/**
 * \brief Evaluate a single pawn.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the pawn is on
 * \param mgscore       a pointer to the middle game score accumulator
 * \param egscore       a pointer to the endgame score accumulator
 *
 */
void eval_pawn(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore)
{
    assert(pos->piece[sq] == PAWN || pos->piece[sq] == -PAWN);

    int32_t pm;

    /* start with the piece square value */
    if (is_white_piece(pos->piece[sq]))
    {
        pm = 1;
        *mgscore += pawn_pst[sq];
        *egscore += pawn_endgame_pst[sq];
    }
    else
    {
        pm = -1;
        int32_t flipsq = flip_rank[sq];
        *mgscore -= pawn_pst[flipsq];
        *egscore -= pawn_endgame_pst[flipsq];
    }

    /* add a bonus for the pawn being passed */
    if (pawn_passed(pos, sq)) 
    {
        int32_t pp = passed_pawn * pm;
        *mgscore += pp;
        *egscore += pp;
    }
    
    /* add a penalty for the pawn being doubled */
    if (pawn_doubled(pos, sq)) 
    {
        int32_t dp = doubled_pawn * pm;
        *mgscore += dp;
        *egscore += dp;
    }

    /* add a penalty for the pawn being isolated */
    if (pawn_isolated(pos, sq)) 
    {
        int32_t ip = isolated_pawn * pm;
        *mgscore += ip;
        *egscore += ip;
    }

}
