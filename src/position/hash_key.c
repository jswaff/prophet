#include "position.h"

#include "prophet/square.h"

#include "bitmap/bitmap.h"
#include "hash/hash_internal.h"
#include "position/square_internal.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>


uint64_t build_hash_key(const position_t *pos)
{
    uint64_t hkey = 0;

    /* fold in pieces */
    for (int i=0;i<64;i++) {
        int32_t piece = pos->piece[i];
        if (piece > NO_PIECE) {
            hkey ^= zkeys.pieces[piece][WHITE][i];
        } else if (piece < NO_PIECE) {
            hkey ^= zkeys.pieces[-piece][BLACK][i];
        }
    }

    /* fold in player to move */
    hkey ^= zkeys.ptm[pos->player];

    /* castling rights */
    assert(pos->castling_rights <= CASTLE_ALL);
    hkey ^= zkeys.casting_rights[pos->castling_rights];

    assert(pos->ep_sq <= NO_SQUARE);
    if (pos->ep_sq != NO_SQUARE) {
        hkey ^= zkeys.ep[pos->ep_sq];
    }

    return hkey;
}


uint64_t build_pawn_key(const position_t *pos)
{
    uint64_t pkey = 0;

    uint64_t pmap = pos->white_pawns;
    while (pmap) {
        square_t sq = (square_t)get_lsb(pmap);
        pkey ^= zkeys.pieces[PAWN][WHITE][sq];
        pmap ^= square_to_bitmap(sq);
    }

    pmap = pos->black_pawns;
    while (pmap) {
        square_t sq = (square_t)get_lsb(pmap);
        pkey ^= zkeys.pieces[PAWN][BLACK][sq];
        pmap ^= square_to_bitmap(sq);
    }

    return pkey;
}
