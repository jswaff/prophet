#include "prophet/movegen.h"

#include "movegen_internal.h"
#include "position/position.h"

#include <assert.h>
#include <stdbool.h>
#include <string.h>


void generate_moves_from_fen(move_t *m, int *n, const char *fen, bool caps, bool noncaps) {
    position_t pos;
    set_pos(&pos, fen);

    move_t *mp = gen_pseudo_legal_moves(m, &pos, caps, noncaps);
    *n = (mp - m);
}


move_t* gen_pseudo_legal_moves(move_t *m, const position_t *pos, bool caps, bool noncaps)
{
    assert(caps || noncaps);

    m = gen_pawn_moves(m, pos, caps, noncaps);
    m = gen_knight_moves(m, pos, caps, noncaps);
    m = gen_bishop_moves(m, pos, caps, noncaps);
    m = gen_rook_moves(m, pos, caps, noncaps);
    m = gen_queen_moves(m, pos, caps, noncaps);
    m = gen_king_moves(m, pos, caps, noncaps);

    return m;
}

move_t* gen_legal_moves(move_t *m, const position_t *p, bool caps, bool noncaps)
{
    assert(caps || noncaps);

    position_t cp;
    memcpy(&cp, p, sizeof(position_t));

    move_t* endp = gen_pseudo_legal_moves(m, &cp, caps, noncaps);
    undo_t u;

    for (move_t *mp=m; mp<endp; mp++) {
        apply_move(&cp, *mp, &u);
        if (in_check(&cp, opposite_player(cp.player))) {
            /* mark as 'invalid' */
            *mp = 0;
        }
        undo_move(&cp, &u);
    }

    return endp;
}
