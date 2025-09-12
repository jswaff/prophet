#include "../movegen/movegen_internal.h"

#include "prophet/move.h"
#include "prophet/movegen.h"
#include "prophet/piece.h"
#include "prophet/square.h"

#include "position/position.h"
#include "position/square_internal.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>


static bool good_pawn_move(const position_t *pos, square_t from_sq, square_t to_sq, int32_t captured, bool is_ep);

#ifndef NDEBUG
static bool verify_good_move(const position_t *pos, move_t mv);
#endif 


bool good_move(const position_t *pos, move_t mv)
{
    bool retval = false;

    square_t from_sq = get_from_sq(mv);
    square_t to_sq = get_to_sq(mv);
    piece_t mover = get_piece(mv);
    piece_t captured_piece = get_captured_piece(mv);

    if (mv == NO_MOVE) {
        return false;
    }

    /* are the squares valid? */
    if (from_sq < A8 || from_sq > H1 || to_sq < A8 || to_sq > H1) {
        return false;
    }

    /* is the piece the correct color for the player on move? */
    int32_t piece_on_board = pos->piece[from_sq];
    if (pos->player==WHITE) {
        if (!is_white_piece(piece_on_board) || (int32_t)mover != piece_on_board) {
            goto exit;
        }
    } else {
        if (!is_black_piece(piece_on_board) || (int32_t)mover != -piece_on_board) {
            goto exit;
        }
    }

    if (mover==PAWN) {
        retval = good_pawn_move(pos, from_sq, to_sq, captured_piece, is_epcapture(mv));
        goto exit;
    }


    /* validate captured piece is right color */
    if (captured_piece != NO_PIECE) {
        if (pos->player==WHITE) {
            if (is_not_black_piece(pos->piece[to_sq])) {
                goto exit;
            }
        } else {
            if (is_not_white_piece(pos->piece[to_sq])) {
                goto exit;
            }
        }
    } else {
        if (pos->piece[to_sq] != NO_PIECE) {
            goto exit;
        }
    }

    /* validate the piece is moving as it should */
    switch (mover) {
        case KNIGHT:
            if (get_knight_moves(from_sq) & square_to_bitmap(to_sq)) {
                retval = true;
            }
            break;
        case BISHOP:
            if (get_bishop_moves(pos, from_sq) & square_to_bitmap(to_sq)) {
                retval = true;
            }
            break;
        case ROOK:
            if (get_rook_moves(pos, from_sq) & square_to_bitmap(to_sq)) {
                retval = true;
            }
            break;
        case QUEEN:
            if (get_queen_moves(pos, from_sq) & square_to_bitmap(to_sq)) {
                retval = true;
            }
            break;
        case KING:
            if (get_king_moves(from_sq) & square_to_bitmap(to_sq)) {
                retval = true;
            } else if (is_castle(mv)) {
                if (to_sq==G1 && can_castle_wk(pos) 
                    && pos->piece[F1]==NO_PIECE && pos->piece[G1]==NO_PIECE
                    && !attacked(pos,E1,BLACK) && !attacked(pos,F1,BLACK))
                {
                    retval = true;
                } else if (to_sq==C1 && can_castle_wq(pos)
                    && pos->piece[D1]==NO_PIECE && pos->piece[C1]==NO_PIECE 
                    && pos->piece[B1]==NO_PIECE
                    && !attacked(pos,E1,BLACK) && !attacked(pos,D1,BLACK))
                {
                    retval = true;
                } else if (to_sq==G8 && can_castle_bk(pos)
                    && pos->piece[F8]==NO_PIECE && pos->piece[G8]==NO_PIECE
                    && !attacked(pos,E8,WHITE) && !attacked(pos,F8,WHITE))
                {
                    retval = true;
                } else if (to_sq==C8 && can_castle_bq(pos)
                    && pos->piece[D8]==NO_PIECE && pos->piece[C8]==NO_PIECE 
                    && pos->piece[B8]==NO_PIECE
                    && !attacked(pos,E8,WHITE) && !attacked(pos,D8,WHITE))
                {
                    retval = true;
                }
            }
            break;
        default:
            break;
    }

exit:

    assert(retval == verify_good_move(pos, mv));
    return retval;
}


static bool good_pawn_move(const position_t *pos, square_t from_sq, square_t to_sq, int32_t captured_piece, bool is_ep)
{
    if (captured_piece==NO_PIECE) {
        if (pos->piece[to_sq] != NO_PIECE) {
            return false;
        }
        if (pos->player==WHITE) {
            square_t nsq = north(from_sq);
            square_t nnsq = north(nsq);
            if (to_sq==nsq || (to_sq==nnsq && pos->piece[nsq]==NO_PIECE)) {
                return true;
            }
        } else {
            square_t ssq = south(from_sq);
            square_t sssq = south(ssq);
            if (to_sq==ssq || (to_sq==sssq && pos->piece[ssq]==NO_PIECE)) {
                return true;
            }
        }
    } else { /* this is a capture */
        if (pos->player==WHITE) {
            if (northeast(from_sq) != to_sq && northwest(from_sq) != to_sq) {
                return false;
            }
            if (is_ep) {
                if (to_sq==pos->ep_sq && pos->piece[south(to_sq)]==-PAWN) {
                    return true;
                }
            } else {
                if (pos->piece[to_sq]==-captured_piece) {
                    return true;
                }
            }
        } else {
            if (southeast(from_sq) != to_sq && southwest(from_sq) != to_sq) {
                return false;
            }
            if (is_ep) {
                if (to_sq==pos->ep_sq && pos->piece[north(to_sq)]==PAWN) {
                    return true;
                }
            } else {
                if (pos->piece[to_sq]==captured_piece) {
                    return true;
                }
            }
        }
    }


    return false;
}

#ifndef NDEBUG
static bool verify_good_move(const position_t *pos, move_t mv)
{
    move_t moves[MAX_MOVES_PER_PLY];
    move_t *endp = gen_pseudo_legal_moves(moves, pos, true, true);
    return move_list_contains(mv, moves, endp);
}
#endif
