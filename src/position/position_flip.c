#include "position_internal.h"

#include <string.h>

/**
 * \brief Flip a position vertically.
 *
 * \param pos           a pointer to a chess position
 *
 * \return a veritical mirror of the posiiton.
 */
position_t position_flip(const position_t* pos)
{
    position_t flipped;
    memset(&flipped, 0, sizeof(position_t));

    /* set the pieces */
    for (int sq = 0; sq < 64; sq++)
    {
        int32_t pc = pos->piece[sq];
        if (pc != NO_PIECE)
        {
            add_piece(&flipped, -pc, flip_rank[sq]);
        }
    }

    /* set the king squares */
    flipped.white_king = flip_rank[pos->black_king];
    flipped.black_king = flip_rank[pos->white_king];

    /* set the player to move */
    flipped.player = pos->player == WHITE ? BLACK : WHITE;

    /* flip ep square */
    if (pos->ep_sq != NO_SQUARE) 
    {
        flipped.ep_sq = flip_rank[pos->ep_sq];
    }
    else
    {
        flipped.ep_sq = NO_SQUARE;
    }

    /* castling rights */
    if (pos->castling_rights & CASTLE_WK)
    {
        flipped.castling_rights |= CASTLE_BK;
    }
    if (pos->castling_rights & CASTLE_WQ)
    {
        flipped.castling_rights |= CASTLE_BQ;
    }
    if (pos->castling_rights & CASTLE_BK)
    {
        flipped.castling_rights |= CASTLE_WK;
    }
    if (pos->castling_rights & CASTLE_BQ)
    {
        flipped.castling_rights |= CASTLE_WQ;
    }

    /* leave the fifty and move counters as is */
    flipped.move_counter = pos->move_counter;
    flipped.fifty_counter = pos->fifty_counter;

    /* rebuild the hash keys */
    flipped.hash_key = build_hash_key(&flipped);
    flipped.pawn_key = build_pawn_key(&flipped);

    /* verify everything is consistent */
    assert(verify_pos(&flipped));


    /* success */

    return flipped;
}