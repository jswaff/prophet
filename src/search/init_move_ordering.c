#include "search_internal.h"

#include <assert.h>
#include <string.h>


/**
 * \brief Initialize move ordering
 *
 * \param mo            pointer to the move ordering data structure
 * \param stackptr      pointer to move stack to place new moves on
 * \param pv_move       pv move
 * \param hash_move     hash move
 * \param killer1       killer move 1
 * \param killer2       killer move 2
 * \param gen_noncaps   whether to generate non-capturing moves
 * \param play_badcaps  whether to play bad captures 
 */
void initialize_move_ordering(move_order_dto* mo, move_t* stackptr,
    move_t pv_move, move_t hash_move, move_t killer1, move_t killer2, 
    bool gen_noncaps, bool play_badcaps)
{
    assert(mo);
    assert(stackptr);
    
    memset(mo, 0, sizeof(move_order_dto));
    mo->next_stage = PV;
    mo->start = mo->current = mo->end = stackptr;
    mo->pv_move = pv_move;
    mo->hash_move = hash_move;
    mo->killer1 = killer1;
    mo->killer2 = killer2;
    mo->gen_noncaps = gen_noncaps;
    mo->play_badcaps = play_badcaps;
}