#include "movegen_internal.h"

#include "prophet/const.h"
#include "prophet/square.h"

#include "position/square_internal.h"

#include <assert.h>
#include <stdint.h>

/**
 * \brief Generate a moves mask in one direction based on the set of occupied 
 * squares
 *
 * From the square \p sq, add all squares in one direction until the edge of 
 * the board or until an occupied square is encountered.
 *
 * \param sq            the square to start from
 * \param occupied      the set of occupied squares
 * \param dir_func      a function giving the next square for a given direction
 *
 * \return the set of squares that can be moved to
 */
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
