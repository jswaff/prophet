#include "position_internal.h"

#include "prophet/position/position.h"

#include <stdbool.h>
#include <stdint.h>

/**
 * \brief Test two positions for equality
 *
 * In strict mode, each member must be equal.  In non-strict mode, that is 
 * relaxed somewhat - the fifty move counter and move counter are not required 
 * to be equal.
 *
 * \param p1            a pointer to a chess position
 * \param p2            a pointer to a second chess position
 * \param strict        strict mode
 *
 * \return a boolean indicating if the positions are equal
 */
bool pos_equals(const position_t* p1, const position_t* p2, bool strict)
{
    for (int32_t sq=0; sq<64; sq++) {
        if (p1->piece[sq] != p2->piece[sq]) {
            return false;
        }
    }
    if (p1->white_pawns != p2->white_pawns) return false;
    if (p1->black_pawns != p2->black_pawns) return false;

    if (p1->white_knights != p2->white_knights) return false;
    if (p1->black_knights != p2->black_knights) return false;

    if (p1->white_bishops != p2->white_bishops) return false;
    if (p1->black_bishops != p2->black_bishops) return false;

    if (p1->white_rooks != p2->white_rooks) return false;
    if (p1->black_rooks != p2->black_rooks) return false;

    if (p1->white_queens != p2->white_queens) return false;
    if (p1->black_queens != p2->black_queens) return false;

    if (p1->white_pieces != p2->white_pieces) return false;
    if (p1->black_pieces != p2->black_pieces) return false;

    /* if that's true then piece counts should be equal */
    if (p1->piece_counts[WHITE][PAWN] != p2->piece_counts[WHITE][PAWN]) 
        return false;
    if (p1->piece_counts[WHITE][ROOK] != p2->piece_counts[WHITE][ROOK]) 
        return false;
    if (p1->piece_counts[WHITE][KNIGHT] != p2->piece_counts[WHITE][KNIGHT]) 
        return false;
    if (p1->piece_counts[WHITE][BISHOP] != p2->piece_counts[WHITE][BISHOP]) 
        return false;
    if (p1->piece_counts[WHITE][QUEEN] != p2->piece_counts[WHITE][QUEEN]) 
        return false;
    if (p1->piece_counts[WHITE][KING] != p2->piece_counts[WHITE][KING]) 
        return false;

    if (p1->piece_counts[BLACK][PAWN] != p2->piece_counts[BLACK][PAWN]) 
        return false;
    if (p1->piece_counts[BLACK][ROOK] != p2->piece_counts[BLACK][ROOK]) 
        return false;
    if (p1->piece_counts[BLACK][KNIGHT] != p2->piece_counts[BLACK][KNIGHT]) 
        return false;
    if (p1->piece_counts[BLACK][BISHOP] != p2->piece_counts[BLACK][BISHOP]) 
        return false;
    if (p1->piece_counts[BLACK][QUEEN] != p2->piece_counts[BLACK][QUEEN]) 
        return false;
    if (p1->piece_counts[BLACK][KING] != p2->piece_counts[BLACK][KING]) 
        return false;

    if (p1->player != p2->player) return false;
    if (p1->ep_sq != p2->ep_sq) return false;
    if (p1->castling_rights != p2->castling_rights) return false;
    if (p1->white_king != p2->white_king) return false;
    if (p1->black_king != p2->black_king) return false;

    if (strict) {
        if (p1->move_counter != p2->move_counter) return false;
        if (p1->fifty_counter != p2->fifty_counter) return false;
    }

    /* if all that is equal the hash keys should be too */
    if (p1->hash_key != p2->hash_key) return false;
    if (p1->pawn_key != p2->pawn_key) return false;

    return true;
}
