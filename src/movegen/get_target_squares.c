#include <assert.h>
#include <stdint.h>

#include <prophet/const.h>
#include <prophet/parameters.h>

#include "movegen_internal.h"

/**
 * \brief Get the set of target squares
 *
 * If \p caps is true, the targets include the opposite player's pieces.  If 
 * \p noncaps is set, targets includes all empty squares.  Note the two 
 * options are not mutually exclusive.
 *
 * \param p             A pointer to a chess position
 * \param caps          If capture targets should be included
 * \param noncaps       If non-capture targets should be included
 *
 * \return The set of target squares
 */
uint64_t get_target_squares(const position* p, bool caps, bool noncaps)
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
