#include "prophet/position.h"

#include "prophet/hash.h"
#include "prophet/move.h"
#include "prophet/square.h"

#include "nn/nn_internal.h"
#include "position_internal.h"
#include "square_internal.h"

#include <assert.h>
#include <stdlib.h>

static piece_t remove_captured_piece(position_t* p, move_t m);
static void add_piece_to_destination(position_t* p, move_t m);
static void remove_castling_availability(position_t* p, move_t mv);
static void remove_rook_castling_availability(position_t* p, square_t sq);

extern neural_network_t neural_network;
extern bool use_neural_network;

/**
 * \brief Apply a chess move to a chess position.
 *
 * Apply a move to the position.  The move should be legal (no verification is 
 * performed). Records the information necessary to undo this move to restore 
 * the original position.
 *
 * \param pos           a pointer to a chess position
 * \param m             the move to apply
 * \param u             a pointer to an undo structure to receive the undo 
 *                      information
 */
void apply_move(position_t* pos, move_t m, undo_t* u)
{
    assert(pos);
    assert(clear_score(m));
    assert(verify_pos(pos));
    assert(u);

    /* capture information to undo move */
    u->mv = m;
    u->hash_key = pos->hash_key;
    u->ep_sq = pos->ep_sq;
    u->fifty_counter = pos->fifty_counter;
    u->castling_rights = pos->castling_rights;

    /* swap player */
    pos->hash_key ^= zkeys.ptm[pos->player];
    pos->player = opposite_player(pos->player);
    pos->hash_key ^= zkeys.ptm[pos->player];
    
    pos->move_counter++;

    if (is_capture(m)) {
        pos->fifty_counter = 0;
        u->captured = remove_captured_piece(pos, m);
    } else {
        pos->fifty_counter++;
        u->captured = NO_PIECE;
    }
    assert(abs((int)u->captured) == (int)get_captured_piece(m));

    /* clear EP square */
    if (pos->ep_sq != NO_SQUARE) {
        pos->hash_key ^= zkeys.ep[pos->ep_sq];
        pos->ep_sq = NO_SQUARE;
    }

    add_piece_to_destination(pos, m);
    remove_castling_availability(pos, m);
    remove_piece(pos, get_from_sq(m));

    /* update accumulators */
    if (use_neural_network) {
        if (get_piece(m) != KING && !is_epcapture(m) && get_promopiece(m)==NO_PIECE) {
            /* incremental update */
            if (u->captured != NO_PIECE) {
                nn_remove_piece(u->captured, pos->player, get_to_sq(m), &neural_network, &pos->nnue_accumulator);
            }
            nn_move_piece(get_piece(m), opposite_player(pos->player), get_from_sq(m), get_to_sq(m),
                &neural_network, &pos->nnue_accumulator);
        } else {
            populate_accumulators(pos, &neural_network);
        }
    }

    assert(verify_pos(pos));
}

static piece_t remove_captured_piece(position_t* p, move_t m)
{
    assert(is_capture(m));
    piece_t captured;

    if (is_epcapture(m)) {
        /* remove pawn */
        if (p->player==WHITE) { 
            /* black WAS on move */
            captured = remove_piece(p, north(p->ep_sq));
            assert(captured == PAWN);
        } else {
            captured = remove_piece(p, south(p->ep_sq));
            assert((int)captured == -PAWN);
        }
    } else {
        captured = remove_piece(p, get_to_sq(m));
    }

    assert(captured != NO_PIECE);

    return captured;
}

/**
 * \brief Add the moving piece to the destination square and apply special 
 * rules.
 *
 * The moving piece is added to the destination square.
 *
 * If the moving piece is a pawn:
 *    - The fifty move counter is reset
 *    - The ep target square is set, if appropriate.
 *    - Promotions are handled.
 *
 * If moving piece is a king:
 *    - The king square is updated.
 *    - If it's a castle, the rook is moved.
 *
 * \param pos           a pointer to a chess position
 * \param m             a move being applied to the position
 */
static void add_piece_to_destination(position_t* p, move_t m)
{
    square_t from_sq = get_from_sq(m), to_sq = get_to_sq(m);
    int32_t piece = p->piece[from_sq];

    add_piece(p, piece, to_sq);

    switch (piece) {
        case PAWN:
            p->fifty_counter = 0;
            if (to_sq == north(north(from_sq))) {
                p->ep_sq = north(from_sq);
                p->hash_key ^= zkeys.ep[p->ep_sq];
            } else if (get_rank(to_sq) == RANK_8) {
                assert(get_promopiece(m) != NO_PIECE);
                remove_piece(p, to_sq);
                add_piece(p, get_promopiece(m), to_sq);
            }
            break;
        case -PAWN:
            p->fifty_counter = 0;
            if (to_sq == south(south(from_sq))) {
                p->ep_sq = south(from_sq);
                p->hash_key ^= zkeys.ep[p->ep_sq];
            } else if (get_rank(to_sq) == RANK_1) {
                assert(get_promopiece(m) != NO_PIECE);
                remove_piece(p, to_sq);
                add_piece(p, -get_promopiece(m), to_sq);
            }
            break;
        case KING:
            p->white_king = to_sq;
            /* move rook if this is a castle */
            if (from_sq == E1) {
                if (to_sq == G1) {
                    assert(is_castle(m));
                    p->fifty_counter = 0;
                    remove_piece(p, H1);
                    add_piece(p, ROOK, F1);
                } else if (to_sq == C1) {
                    assert(is_castle(m));
                    p->fifty_counter = 0;
                    remove_piece(p, A1);
                    add_piece(p, ROOK, D1);
                }
            }
            break;
        case -KING:
            p->black_king = to_sq;
            if (from_sq == E8) {
                if (to_sq == G8) {
                    assert(is_castle(m));
                    p->fifty_counter = 0;
                    remove_piece(p, H8);
                    add_piece(p, -ROOK, F8);
                } else if (to_sq == C8) {
                    assert(is_castle(m));
                    p->fifty_counter = 0;
                    remove_piece(p, A8);
                    add_piece(p, -ROOK, D8);
                }
            }
            break;
    }
}

static void remove_castling_availability(position_t* p, move_t mv)
{
    /* clear current castling rights from hash key */
    p->hash_key ^= zkeys.casting_rights[p->castling_rights];

    /* if capturing a rook remove its castling availability */
    if (is_capture(mv)) {
        square_t to_sq = get_to_sq(mv);
        remove_rook_castling_availability(p, to_sq);
    }

    /* if a rook or king is moving, remove their castling availability. */
    square_t from_sq = get_from_sq(mv);
    int32_t piece = p->piece[from_sq];
    if (piece == KING) {
        p->castling_rights &= CASTLE_BLACK;
    } else if (piece == -KING) {
        p->castling_rights &= CASTLE_WHITE;
    } else if (piece == ROOK || piece == -ROOK) {
        remove_rook_castling_availability(p, from_sq);
    }

    /* add current castling rights to hash key */
    p->hash_key ^= zkeys.casting_rights[p->castling_rights];
}

static void remove_rook_castling_availability(position_t* p, square_t sq)
{
    if (sq == A1) {
        p->castling_rights &= CASTLE_NOT_WQ;
    } else if (sq == H1) {
        p->castling_rights &= CASTLE_NOT_WK;
    } else if (sq == A8) {
        p->castling_rights &= CASTLE_NOT_BQ;
    } else if (sq == H8) {
        p->castling_rights &= CASTLE_NOT_BK;
    }
}
