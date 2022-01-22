#include <prophet/parameters.h>

#include "eval_internal.h"

#include  <assert.h>

/**
 * \brief Evaluate a single pawn.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the pawn is on
 * \param endgame       if the eval should be done in the endgame phase 
 *
 * \return a score for the pawn.
 */
int32_t eval_pawn(const position_t* pos, square_t sq, bool UNUSED(endgame))
{
    assert(pos->piece[sq] == PAWN || pos->piece[sq] == -PAWN);

    int32_t score = 0; 

    /* start with the piece square value */
    if (is_white_piece(pos->piece[sq]))
    {
        score = pawn_pst[sq];
    }
    else
    {
        score = pawn_pst[flip_rank[sq]];
    }

    /* add a bonus for the pawn being passed */
    if (pawn_passed(pos, sq)) 
    {
        score += passed_pawn;
    }
    
    /* add a penalty for the pawn being doubled */
    if (pawn_doubled(pos, sq)) 
    {
        score += doubled_pawn;
    }

    /* add a penalty for the pawn being isolated */
    if (pawn_isolated(pos, sq)) 
    {
        score += isolated_pawn;
    }
    
    return score;    
}
