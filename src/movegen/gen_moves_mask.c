#include "movegen_internal.h"

#include "prophet/const.h"
#include "prophet/square.h"

#include "position/square_internal.h"

#include <assert.h>
#include <stdint.h>


uint64_t gen_moves_mask(square_t sq, uint64_t occupied, dir_func_t dir_func)
{
    uint64_t mask = 0;

    square_t to = dir_func(sq);

    while (to != NO_SQUARE) {
        uint64_t bb_to = square_to_bitmap(to);
        mask |= bb_to;
        if (bb_to & occupied) {
            break;
        }
        to = dir_func(to);
    }

    return mask;
}
