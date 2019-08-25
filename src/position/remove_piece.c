#include <prophet/hash.h>

#include <assert.h>

#include "position_internal.h"

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

    if (piece > NO_PIECE) 
    {
        p->white_pieces ^= bb_sq;

        if (piece==PAWN) 
        {
            p->white_pawns ^= bb_sq;
            p->pawn_key ^= zkeys.pieces[PAWN][WHITE][sq];
        } 
        else if (piece==KNIGHT) 
        {
            p->white_knights ^= bb_sq;
        } 
        else if (piece==BISHOP) 
        {
            p->white_bishops ^= bb_sq;
        } 
        else if (piece==ROOK) 
        {
            p->white_rooks ^= bb_sq;
        } 
        else if (piece==QUEEN) 
        {
            p->white_queens ^= bb_sq;
        }

        p->piece_counts[WHITE][piece]--;
        p->hash_key ^= zkeys.pieces[piece][WHITE][sq];

    } 
    else 
    {
        p->black_pieces ^= bb_sq;

        if (piece==-PAWN) 
        {
            p->black_pawns ^= bb_sq;
            p->pawn_key ^= zkeys.pieces[PAWN][BLACK][sq];
        } 
        else if (piece==-KNIGHT) 
        {
            p->black_knights ^= bb_sq;
        } 
        else if (piece==-BISHOP) 
        {
            p->black_bishops ^= bb_sq;
        } 
        else if (piece==-ROOK) 
        {
            p->black_rooks ^= bb_sq;
        } 
        else if (piece==-QUEEN) 
        {
            p->black_queens ^= bb_sq;
        }

        p->piece_counts[BLACK][-piece]--;
        p->hash_key ^= zkeys.pieces[-piece][BLACK][sq];
    }

    p->piece[sq] = NO_PIECE;

    return (piece_t)piece;
}
