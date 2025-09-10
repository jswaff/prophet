#include "position_internal.h"

#include "prophet/const.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>


bool is_draw_rep(const position_t *pos, const undo_t *u, int prev_reps)
{
    assert(pos);
    assert(u);
    assert(pos->move_counter <= MAX_HALF_MOVES_PER_GAME);
    assert(pos->fifty_counter <= pos->move_counter);
    assert(pos->hash_key == build_hash_key(pos));

    int reps = 0;
    uint32_t start_ind = pos->move_counter - pos->fifty_counter;
    for (uint32_t i = start_ind; i < pos->move_counter; i++) {
        if ((u + i)->hash_key == pos->hash_key) {
            ++reps;
        }
    }

    return reps >= prev_reps;
}
