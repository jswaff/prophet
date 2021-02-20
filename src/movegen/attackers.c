#include <prophet/bitmap.h>
#include <prophet/parameters.h>

#include <assert.h>

#include "movegen_internal.h"

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

    /* fold in knight attackers */
	attackers_map |= get_knight_moves(
        sq, (player==WHITE ? pos->white_knights : pos->black_knights));

    /* fold in king attackers */
	attackers_map |= get_king_moves(
        sq, 
        (player==WHITE ? 
        	square_to_bitmap(pos->white_king) : 
        	square_to_bitmap(pos->black_king)));

	/* fold in bishop attackers */
	attackers_map |= get_bishop_moves(
		pos, sq, (player==WHITE ? pos->white_bishops : pos->black_bishops));

	/* fold in rook attackers */
	attackers_map |= get_rook_moves(
		pos, sq, (player==WHITE ? pos->white_rooks : pos->black_rooks));

	/* fold in queen attackers */
	attackers_map |= get_queen_moves(
		pos, sq, (player==WHITE ? pos->white_queens : pos->black_queens));

	/* fold in pawn attackers */
	if (player==WHITE)
	{
		attackers_map |= ((square_to_bitmap(sq) & ~file_to_bitmap(FILE_A)) << 7) & 
			pos->white_pawns;
		attackers_map |= ((square_to_bitmap(sq) & ~file_to_bitmap(FILE_H)) << 9) & 
			pos->white_pawns;
	}
	else
	{
		attackers_map |= ((square_to_bitmap(sq) & ~file_to_bitmap(FILE_A)) >> 9) & 
			pos->black_pawns;
		attackers_map |= ((square_to_bitmap(sq) & ~file_to_bitmap(FILE_H)) >> 7) & 
			pos->black_pawns;
	}

    return attackers_map;
}
