#include <prophet/hash.h>

#include <assert.h>

#include "position_internal.h"

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
        if (piece == PAWN) {
            p->white_pawns |= bb_sq;
            p->pawn_key ^= zkeys.pieces[PAWN][WHITE][sq];
        } else if (piece == KNIGHT) {
            p->white_knights |= bb_sq;
        } else if (piece == BISHOP) {
            p->white_bishops |= bb_sq;
        } else if (piece == ROOK) {
            p->white_rooks |= bb_sq;
        } else if (piece == QUEEN) {
            p->white_queens |= bb_sq;
        }
        p->piece_counts[WHITE][piece]++;
        p->hash_key ^= zkeys.pieces[piece][WHITE][sq];
    } else {
        p->black_pieces |= bb_sq;
        if (piece == -PAWN) {
            p->black_pawns |= bb_sq;
            p->pawn_key ^= zkeys.pieces[PAWN][BLACK][sq];
        } else if (piece == -KNIGHT) {
            p->black_knights |= bb_sq;
        } else if (piece == -BISHOP) {
            p->black_bishops |= bb_sq;
        } else if (piece == -ROOK) {
            p->black_rooks |= bb_sq;
        } else if (piece == -QUEEN) {
            p->black_queens |= bb_sq;
        }
        p->piece_counts[BLACK][-piece]++;
        p->hash_key ^= zkeys.pieces[-piece][BLACK][sq];
    }
}
