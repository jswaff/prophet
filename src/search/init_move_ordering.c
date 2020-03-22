#include "search_internal.h"

#include <string.h>


void initialize_move_ordering(move_order_dto* mo, move_t* stackptr,
    move_t killer1, move_t killer2)
{

    memset(mo, 0, sizeof(move_order_dto));
    mo->next_stage = GEN_CAPS;
    mo->start = stackptr;
    mo->killer1 = killer1;
    mo->killer2 = killer2;
}