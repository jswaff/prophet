#include "movegen_internal.h"

#include "position/position_internal.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>


uint64_t get_target_squares(const position_t *p, bool caps, bool noncaps)
{
    uint64_t targets = caps ? (p->player == WHITE ? p->black_pieces : p->white_pieces) : 0;

    targets |= noncaps ? ~(p->white_pieces | p->black_pieces) : 0;

    return targets;
}
