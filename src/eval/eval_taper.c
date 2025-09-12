#include "eval_internal.h"

#include "position/position.h"

#include <stdint.h>

/* forward decls */
static int32_t eval_phase(const position_t *pos);


int32_t eval_taper(const position_t *pos, int32_t mg_score, int32_t eg_score)
{
    int32_t mg_phase = eval_phase(pos);
    int32_t eg_phase = 24 - mg_phase;

    return (mg_score * mg_phase + eg_score * eg_phase) / 24;
}


static int32_t eval_phase(const position_t *pos)
{
	int32_t phase = 
        (pos->piece_counts[0][QUEEN] + pos->piece_counts[1][QUEEN]) * 4 + 
        (pos->piece_counts[0][ROOK] + pos->piece_counts[1][ROOK]) * 2 + 
        pos->piece_counts[0][BISHOP] + pos->piece_counts[1][BISHOP] + 
        pos->piece_counts[0][KNIGHT] + pos->piece_counts[1][KNIGHT];

    return phase > 24 ? 24 : phase;
}
