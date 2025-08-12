#include "movegen_internal.h"

#include "prophet/position.h"
#include "prophet/square.h"

#include "position/position_internal.h"
#include "position/square_internal.h"

#include <stdint.h>

/**
 * \brief Given position \p pos, get a bitmap of all pieces of color \p player
 * that are attacking square \p sq.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square in question
 * \param player        the attacking player
 *
 * \return bitmap of all attacking squares
 */
uint64_t attackers(const position_t* pos, square_t sq, color_t player)
{
    uint64_t attackers_map = 0;

    if (player==WHITE) {
        attackers_map |= get_knight_moves(sq) & pos->white_knights;
        attackers_map |= get_king_moves(sq) & square_to_bitmap(pos->white_king);
        attackers_map |= get_bishop_moves(pos, sq, pos->white_bishops);
        attackers_map |= get_rook_moves(pos, sq, pos->white_rooks);
        attackers_map |= get_queen_moves(pos, sq, pos->white_queens);
        attackers_map |= get_pawn_attacks(sq, BLACK) & pos->white_pawns;
    } else {
        attackers_map |= get_knight_moves(sq) & pos->black_knights;
        attackers_map |= get_king_moves(sq) & square_to_bitmap(pos->black_king);
        attackers_map |= get_bishop_moves(pos, sq, pos->black_bishops);
        attackers_map |= get_rook_moves(pos, sq, pos->black_rooks);
        attackers_map |= get_queen_moves(pos, sq, pos->black_queens);
        attackers_map |= get_pawn_attacks(sq, WHITE) & pos->black_pawns;
    }

    return attackers_map;
}
