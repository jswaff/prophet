#include "eval_internal.h"

#include "prophet/piece.h"
#include "prophet/position.h"
#include "prophet/square.h"

#include "bitmap/bitmap.h"
#include "movegen/movegen_internal.h"

#include <assert.h>
#include <stdint.h>


/**
 * \brief Evaluate a connected major on the 7th rank.
 *
 * A major piece is a rook or a queen.  For black, the "7th rank" is the
 * "2nd rank".
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the major piece is on
 * \param mgscore       a pointer to the middle game score accumulator
 * \param egscore       a pointer to the endgame score accumulator
 *
 */
void eval_major_on_7th(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore)
{
    assert(pos->piece[sq] == ROOK || pos->piece[sq] == -ROOK
        || pos->piece[sq] == QUEEN || pos->piece[sq] == -QUEEN);


    if (is_white_piece(pos->piece[sq])) {
        if (get_rank(sq)==RANK_7 && get_rank(pos->black_king)==RANK_8) {
            *mgscore += major_on_7th_mg;
            *egscore += major_on_7th_eg;
            uint64_t rook_moves = get_rook_moves(pos, sq, sq_to_rank_bitmap(sq));
            if (rook_moves & (pos->white_rooks | pos->white_queens)) {
                *mgscore += connected_majors_on_7th_mg;
                *egscore += connected_majors_on_7th_eg;
            }
        }
    } else {
        if (get_rank(sq)==RANK_2 && get_rank(pos->white_king)==RANK_1) {
            *mgscore -= major_on_7th_mg;
            *egscore -= major_on_7th_eg;
            uint64_t rook_moves = get_rook_moves(pos, sq, sq_to_rank_bitmap(sq));
            if (rook_moves & (pos->black_rooks | pos->black_queens)) {
                *mgscore -= connected_majors_on_7th_mg;
                *egscore -= connected_majors_on_7th_eg;
            }
        }
    }
}
