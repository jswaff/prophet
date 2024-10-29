#include "movegen_internal.h"

#include "prophet/position.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * \brief Get the set of target squares
 *
 * If \p caps is true, the targets include the opposite player's pieces.  If 
 * \p noncaps is set, targets includes all empty squares.  Note the two 
 * options are not mutually exclusive.
 *
 * \param p             a pointer to a chess position
 * \param caps          if capture targets should be included
 * \param noncaps       if non-capture targets should be included
 *
 * \return the set of target squares
 */
uint64_t get_target_squares(const position_t* p, bool caps, bool noncaps)
{
    uint64_t targets = 0;

    if (caps) {
        targets = p->player == WHITE ? p->black_pieces : p->white_pieces;
    }

    if (noncaps) {
        targets |= ~(p->white_pieces | p->black_pieces);
    }

    return targets;
}
