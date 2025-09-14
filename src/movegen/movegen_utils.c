#include "movegen_internal.h"

#include "prophet/const.h"

#include "position/position.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>


uint32_t num_legal_moves(const position_t *pos, bool caps, bool noncaps)
{
    move_t moves[MAX_MOVES_PER_PLY];
    move_t *endp = gen_legal_moves(moves, pos, caps, noncaps);

    /* count the number of moves to choose from */
    int num_caps, num_noncaps;
    num_moves_in_list(moves, endp, &num_caps, &num_noncaps);

    uint32_t nmoves = 0;
    if (caps) nmoves += num_caps;
    if (noncaps) nmoves += num_noncaps;

    return nmoves;
}


void num_moves_in_list(const move_t *startp, const move_t *endp, int *caps, int *noncaps)
{
    *caps = 0; *noncaps = 0;

    for (const move_t *mp=startp; mp<endp; mp++) {
        if (*mp != 0) {
            if (is_capture(*mp) || get_promopiece(*mp)) {
                (*caps)++;
            } else {
                (*noncaps)++;
            }
        }
    }
}


bool in_check(const position_t *pos, color_t player)
{
    square_t king_sq = player==WHITE ? pos->white_king : pos->black_king;

    return attacked(pos, king_sq, opposite_player(player));
}


bool is_checkmate(const position_t *pos)
{
    if (in_check(pos, pos->player)) {
        return num_legal_moves(pos, true, true) == 0;
    } 

    return false;
}


bool is_stalemate(const position_t *pos)
{
    if (!in_check(pos, pos->player)) {
        return num_legal_moves(pos, true, true) == 0;
    } 

    return false;
}


bool is_legal_move(move_t mv, const position_t *pos)
{
    move_t moves[MAX_MOVES_PER_PLY];
    move_t* endp = gen_legal_moves(moves, pos, true, true);

    return move_list_contains(mv, moves, endp);
}


bool move_list_contains(move_t mv, const move_t *start, const move_t *end)
{
    for (const move_t *mp=start; mp<end; mp++) {
        if (clear_score(*mp) == clear_score(mv)) {
            return true;
        }
    }

    return false;
}
