#include "position.h"

#include "prophet/square.h"

#include "hash/hash_internal.h"
#include "square_internal.h"

#include <assert.h>
#include <stdint.h>


/**
 * \brief Add a piece to a chess position
 *
 * \param p             a pointer to a chess position
 * \param piece         the piece to add
 * \param sq            the square to add the piece to
 */
void add_piece(position_t *p, int32_t piece, square_t sq)
{
    assert(piece != NO_PIECE);
    assert(sq >= 0 && sq < 64);
    assert(p->piece[sq] == NO_PIECE);

    p->piece[sq] = piece;
    uint64_t bb_sq = square_to_bitmap(sq);

    if (piece > NO_PIECE) {
        p->white_pieces |= bb_sq;
        switch (piece) {
            case PAWN:
                p->white_pawns |= bb_sq;
                p->pawn_key ^= zkeys.pieces[PAWN][WHITE][sq];
                break;
            case KNIGHT:
                p->white_knights |= bb_sq;
                break;
            case BISHOP:
                p->white_bishops |= bb_sq;
                break;
            case ROOK:
                p->white_rooks |= bb_sq;
                break;
            case QUEEN:
                p->white_queens |= bb_sq;
                break;
        }
        p->piece_counts[WHITE][piece]++;
        p->hash_key ^= zkeys.pieces[piece][WHITE][sq];
    } else {
        p->black_pieces |= bb_sq;
        switch(piece) {
            case -PAWN:
                p->black_pawns |= bb_sq;
                p->pawn_key ^= zkeys.pieces[PAWN][BLACK][sq];
                break;
            case -KNIGHT:
                p->black_knights |= bb_sq;
                break;
            case -BISHOP:
                p->black_bishops |= bb_sq;
                break;
            case -ROOK:
                p->black_rooks |= bb_sq;
                break;
            case -QUEEN:
                p->black_queens |= bb_sq;
                break;
        }
        p->piece_counts[BLACK][-piece]++;
        p->hash_key ^= zkeys.pieces[-piece][BLACK][sq];
    }
}
