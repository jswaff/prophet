#include "movegen/movegen_internal.h"

#include "prophet/const.h"
#include "prophet/movegen.h"

#include "position/position.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>


uint64_t perft(position_t *pos, uint32_t depth)
{
    uint64_t nodes=0;
    move_t moves[MAX_MOVES_PER_PLY];
    undo_t u;

    move_t *endp = gen_legal_moves(moves, pos, true, true);

    if (depth == 1) {
        int caps; int noncaps;
        num_moves_in_list(moves, endp, &caps, &noncaps);
        return caps + noncaps;
    }

    for (move_t *mp=moves; mp<endp; mp++) {
        if (*mp==0) continue; /* move purged as illegal */
        apply_move(pos, *mp, &u);
        nodes += perft(pos, depth-1);
        undo_move(pos, &u);
    }

    return nodes;
}
