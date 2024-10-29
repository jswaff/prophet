#include "eval_internal.h"

#include "prophet/movegen.h"
#include "prophet/piece.h"
#include "prophet/position.h"

#include "../bitmap/bitmap.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

/* forward decls */
static void eval_rook_open_file(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore);


/**
 * \brief Evaluate a single rook.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the rook is on
 * \param mgscore       a pointer to the middle game score accumulator
 * \param egscore       a pointer to the endgame score accumulator
 *
 */
void eval_rook(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore)
{
    assert(pos->piece[sq] == ROOK || pos->piece[sq] == -ROOK);

    uint64_t empty_squares = ~(pos->white_pieces | pos->black_pieces);
    uint32_t mobility = popcnt(get_rook_moves(pos, sq, empty_squares));
    uint32_t mobility_mg = rook_mobility_mg[mobility];
    uint32_t mobility_eg = rook_mobility_eg[mobility];

    if (is_white_piece(pos->piece[sq])) {
        *mgscore += rook_pst_mg[sq] + mobility_mg;
        *egscore += rook_pst_eg[sq] + mobility_eg;
    } else {
        int32_t flipsq = flip_rank[sq];
        *mgscore -= (rook_pst_mg[flipsq] + mobility_mg);
        *egscore -= (rook_pst_eg[flipsq] + mobility_eg);
    }

    eval_major_on_7th(pos, sq, mgscore, egscore);
    eval_rook_open_file(pos, sq, mgscore, egscore);
}


/**
 * \brief Evaluate a square for an open or half open file.
 *
 * An open file is defined as a file with no pawns.  A half open file is 
 * defined as a file with no friendly pawns, but at least one enemy pawn.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the rook is on
 * \param mgscore       a pointer to the middle game score accumulator
 * \param egscore       a pointer to the endgame score accumulator
 *
 */ 
static void eval_rook_open_file(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore)
{
    uint64_t friendly_pawns, enemy_pawns /*, friendly_rooks */;
    bool wtm = is_white_piece(pos->piece[sq]);
    if (wtm) {
        friendly_pawns = pos->white_pawns;
        enemy_pawns = pos->black_pawns;
        /* friendly_rooks = pos->white_rooks; */
    } else {
        friendly_pawns = pos->black_pawns;
        enemy_pawns = pos->white_pawns;
        /* friendly_rooks = pos->black_rooks; */
    }

    uint64_t file_mask = sq_to_file_bitmap(sq);
    if (!(file_mask & friendly_pawns)) {
        /*uint64_t rook_file_moves = get_rook_moves(pos, sq, sq_to_file_bitmap(sq));*/
        if (file_mask & enemy_pawns) {
            if (wtm) {
                *mgscore += rook_half_open_file_mg;
                *egscore += rook_half_open_file_eg;
                /*if (rook_file_moves & friendly_rooks) {
                    *mgscore += rook_half_open_file_supported_mg;
                    *egscore += rook_half_open_file_supported_eg;
                }*/
            } else {
                *mgscore -= rook_half_open_file_mg;
                *egscore -= rook_half_open_file_eg;
                /*if (rook_file_moves & friendly_rooks) {
                    *mgscore -= rook_half_open_file_supported_mg;
                    *egscore -= rook_half_open_file_supported_eg;
                }*/
            }
        } else {
            if (wtm) {
                *mgscore += rook_open_file_mg;
                *egscore += rook_open_file_eg;
                /*if (rook_file_moves & friendly_rooks) {
                    *mgscore += rook_open_file_supported_mg;
                    *egscore += rook_open_file_supported_eg;
                }*/
            } else {
                *mgscore -= rook_open_file_mg;
                *egscore -= rook_open_file_eg;
                /*if (rook_file_moves & friendly_rooks) {
                    *mgscore -= rook_open_file_supported_mg;
                    *egscore -= rook_open_file_supported_eg;
                }*/
            }
        }
    }

}
