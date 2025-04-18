#include "position_internal.h"

#include "prophet/hash.h"
#include "prophet/position.h"
#include "prophet/square.h"

#include "position/square_internal.h"

#include <assert.h>
#include <stdint.h>

/**
 * \brief Remove a piece from a chess position
 *
 * \param p             a pointer to a chess position
 * \param sq            the square to remove the piece from
 *
 * \return the piece that was removed
 */
piece_t remove_piece(position_t* p,square_t sq)
{
    assert(p != 0);
    assert(sq >= A8 && sq <= H1);
    assert(p->piece[sq] != NO_PIECE);

    int32_t piece = p->piece[sq];
    uint64_t bb_sq = square_to_bitmap(sq);

    if (piece > NO_PIECE) {
        p->white_pieces ^= bb_sq;

        switch(piece) {
            case PAWN:
                p->white_pawns ^= bb_sq;
                p->pawn_key ^= zkeys.pieces[PAWN][WHITE][sq];
                break;
            case KNIGHT:
                p->white_knights ^= bb_sq;
                break;
            case BISHOP:
                p->white_bishops ^= bb_sq;
                break;
            case ROOK:
                p->white_rooks ^= bb_sq;
                break;
            case QUEEN:
                p->white_queens ^= bb_sq;            
                break;
        }

        p->piece_counts[WHITE][piece]--;
        p->hash_key ^= zkeys.pieces[piece][WHITE][sq];

    } else {
        p->black_pieces ^= bb_sq;

        switch(piece) {
            case -PAWN:
                p->black_pawns ^= bb_sq;
                p->pawn_key ^= zkeys.pieces[PAWN][BLACK][sq];
                break;
            case -KNIGHT:
                p->black_knights ^= bb_sq;
                break;
            case -BISHOP:
                p->black_bishops ^= bb_sq;
                break;
            case -ROOK:
                p->black_rooks ^= bb_sq;
                break;
            case -QUEEN:
                p->black_queens ^= bb_sq;
                break;
        }

        p->piece_counts[BLACK][-piece]--;
        p->hash_key ^= zkeys.pieces[-piece][BLACK][sq];
    }

    p->piece[sq] = NO_PIECE;

    return (piece_t)piece;
}
