#include "prophet/movegen.h"

#include "prophet/const.h"
#include "prophet/position.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * \brief Count all possible moves to a fixed depth.
 *
 * The term 'perft' was first introduced by Bob Hyatt of Cray Blitz and Crafty 
 * fame. It's basically a functional test that walks the move generation tree 
 * in depth first fashion, and returning a node (vertex) count when complete.  
 * Tested from a wide range of positions, this gives a very high level of 
 * confidence that the move generation and apply functions are working 
 * correctly.
 *
 * Another use of 'perft', (and perhaps Hyatt's intention given the name) is 
 * to measure the performance of the move gen/apply systems.  By recording the 
 * time it takes to complete a perft run and the node count it returns, we have
 * a metric -- nodes per second.
 *
 * Reference: http://chessprogramming.wikispaces.com/Perft
 *
 * \param pos           a pointer to a chess position
 * \param depth         the depth of the tree from this position
 *
 * \return the number of moves generated
 */
uint64_t perft(position_t* pos, uint32_t depth)
{
    uint64_t nodes=0;
    move_t moves[MAX_MOVES_PER_PLY];
    undo_t u;

    move_t* endp = gen_legal_moves(moves, pos, true, true);

    if (depth == 1) {
        int caps; int noncaps;
        num_moves_in_list(moves, endp, &caps, &noncaps);
        return caps + noncaps;
    }

    for (move_t* mp=moves; mp<endp; mp++) {
        if (*mp==0) continue; /* move purged as illegal */
        apply_move(pos, *mp, &u);
        nodes += perft(pos, depth-1);
        undo_move(pos, &u);
    }

    return nodes;
}
