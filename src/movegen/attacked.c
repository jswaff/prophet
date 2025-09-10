#include "movegen_internal.h"

#include "prophet/square.h"

#include "bitmap/bitmap.h"
#include "position/position.h"
#include "position/square_internal.h"

#include <assert.h>
#include <stdbool.h>


bool attacked(const position_t *pos, square_t sq, color_t player)
{
    assert(pos);
    assert(sq >= A8 && sq <= H1);
    assert(player==WHITE || player==BLACK);

    if (player==WHITE) {
        return (get_pawn_attacks(sq, BLACK) & pos->white_pawns) || 
            (get_knight_moves(sq) & pos->white_knights) ||
            (get_king_moves(sq) & square_to_bitmap(pos->white_king)) ||
            (get_rook_moves(pos, sq) & (pos->white_rooks | pos->white_queens)) ||
            (get_bishop_moves(pos, sq) & (pos->white_bishops | pos->white_queens));
    } else {
        return (get_pawn_attacks(sq, WHITE) & pos->black_pawns) ||
            (get_knight_moves(sq) & pos->black_knights) ||
            (get_king_moves(sq) & square_to_bitmap(pos->black_king)) ||
            (get_rook_moves(pos, sq) & (pos->black_rooks | pos->black_queens)) || 
            (get_bishop_moves(pos, sq) & (pos->black_bishops | pos->black_queens));
    }
}


bool attacked_by_bishop(const position_t *p, square_t sq, color_t player)
{
    assert(p);
    assert(sq >= A8 && sq <= H1);
    assert(player==WHITE || player==BLACK);

    return get_bishop_moves(p, sq) & (player==WHITE ? p->white_bishops : p->black_bishops);
}


bool attacked_by_king(const position_t *p, square_t sq, color_t player)
{
    assert(p);
    assert(sq >= A8 && sq <= H1);
    assert(player==WHITE || player==BLACK);

    return get_king_moves(sq) &  square_to_bitmap(player==WHITE ? p->white_king : p->black_king);
}


bool attacked_by_knight(const position_t *p, square_t sq, color_t player)
{
    assert(p);
    assert(sq >= A8 && sq <= H1);
    assert(player==WHITE || player==BLACK);

    return get_knight_moves(sq) & (player==WHITE ? p->white_knights : p->black_knights);
}


bool attacked_by_pawn(const position_t *p, square_t sq, color_t player)
{
    assert(p);
    assert(sq >= A8 && sq <= H1);
    assert(player==WHITE || player==BLACK);

    if (player==WHITE) {
        return get_pawn_attacks(sq, BLACK) & p->white_pawns;
    } else {
        return get_pawn_attacks(sq, WHITE) & p->black_pawns;
    }
}


bool attacked_by_queen(const position_t *p, square_t sq, color_t player)
{
    assert(p);
    assert(sq >= A8 && sq <= H1);
    assert(player==WHITE || player==BLACK);

    return get_queen_moves(p, sq) & (player==WHITE ? p->white_queens : p->black_queens);
}


bool attacked_by_rook(const position_t *p, square_t sq, color_t player)
{
    assert(p);
    assert(sq >= A8 && sq <= H1);
    assert(player==WHITE || player==BLACK);

    return get_rook_moves(p, sq) & (player==WHITE ? p->white_rooks : p->black_rooks);
}
