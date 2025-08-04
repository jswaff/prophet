#include "prophet/position.h"

#include "prophet/move.h"
#include "prophet/square.h"

#include "nn/nn_internal.h"
#include "position_internal.h"

#include <assert.h>
#include <stdlib.h>

extern neural_network_t neural_network;
extern bool use_neural_network;

/**
 * \brief Undo (reverse) a move.
 *
 * The move should be the last move played over the board.  No verification 
 * is done to ensure the position is left in a legal state.
 *
 * \param pos           a pointer to a chess position
 * \param u             a pointer to the undo information
 */
void undo_move(position_t* p, const undo_t* u)
{
    assert(p);
    assert(p->move_counter > 0);
    assert(u);

    p->player = opposite_player(p->player);
    p->ep_sq = u->ep_sq;
    p->castling_rights = u->castling_rights;
    p->fifty_counter = u->fifty_counter;
    p->move_counter--;

    move_t mv = u->mv;
    assert(abs((int)u->captured) == (int)get_captured_piece(mv));

    /* remove the moving piece from the destination square */
    square_t to_sq = get_to_sq(mv);
    square_t from_sq = get_from_sq(mv);
    piece_t piece = get_piece(mv);

    if (p->piece[to_sq] != NO_PIECE) {
        remove_piece(p, to_sq);
    }

    /* place the moving piece on from_sq */
    add_piece(p, piece * (p->player==WHITE ? 1 : -1), from_sq);
    if (piece == KING) {
        if (p->player == WHITE) {
            p->white_king = from_sq;
        } else {
            p->black_king = from_sq;
        }
    }

    /* restore the captured piece */
    if (is_capture(mv)) {
        if (is_epcapture(mv)) {
            if (p->player==BLACK) {
                add_piece(p, PAWN, (square_t)(p->ep_sq-8));
            } else {
                add_piece(p, -PAWN, (square_t)(p->ep_sq+8));
            }
        } else {
            add_piece(p, u->captured, to_sq);
        }
    } else if (is_castle(mv)) {
        if (from_sq == E1) {
            if (to_sq == G1) {
                remove_piece(p, F1);
                add_piece(p, ROOK, H1);
            } else {
                remove_piece(p, D1);
                add_piece(p, ROOK, A1);
            }
        } else {
            if (to_sq == G8) {
                remove_piece(p, F8);
                add_piece(p, -ROOK, H8);
            } else {
                remove_piece(p, D8);
                add_piece(p, -ROOK, A8);
            }
        }
    }

    p->hash_key = u->hash_key;

    /* update accumulators */
    if (use_neural_network) {
        if (!is_castle(mv) && !is_epcapture(mv) && get_promopiece(mv)==NO_PIECE) {
            /* incremental update */
            nn_move_piece(piece, p->player, to_sq, from_sq, &neural_network, &p->nnue_accumulator);
            if (u->captured != NO_PIECE) {
                nn_add_piece(u->captured, opposite_player(p->player), to_sq, &neural_network, &p->nnue_accumulator);
            }
        } else {
            populate_accumulators(p, &neural_network);
        }
    }
}
