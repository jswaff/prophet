#include "search_internal.h"

#include <assert.h>
#include <string.h>


void initialize_move_ordering(move_order_dto *mo, move_t *stackptr, move_t pv_move, move_t hash_move, move_t killer1, 
    move_t killer2, bool gen_noncaps, bool play_badcaps)
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