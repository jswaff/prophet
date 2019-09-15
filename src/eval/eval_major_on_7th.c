#include <prophet/bitmap.h>

#include "eval_internal.h"

/* FIXME */
#include "../movegen/movegen_internal.h"

#include  <assert.h>

/**
 * \brief Evaluate a connected major on the 7th rank.
 *
 * A major piece is a rook or a queen.  For black, the "7th rank" is the
 * "2nd rank".
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the major piece is on
 *
 * \return a score 
 */
int32_t eval_major_on_7th(const position_t* pos, square_t sq)
{
    assert(pos->piece[sq] == ROOK || pos->piece[sq] == -ROOK
        || pos->piece[sq] == QUEEN || pos->piece[sq] == -QUEEN);

    int32_t score = 0; 

    /* get the rook moves to the east in order to look for connected majors */
    uint64_t rook_moves = get_rook_moves(pos, sq, ray(sq, EAST));

    if (is_white_piece(pos->piece[sq]))
    {
        if (get_rank(sq)==RANK_7 && get_rank(pos->black_king)==RANK_8)
        {
            score += major_on_7th;
            if (rook_moves & (pos->white_rooks | pos->white_queens))
            {
                score += connected_majors_on_7th;
            }
        }
    }
    else
    {
        if (get_rank(sq)==RANK_2 && get_rank(pos->white_king)==RANK_1)
        {
            score += major_on_7th;
            if (rook_moves & (pos->black_rooks | pos->black_queens))
            {
                score += connected_majors_on_7th;
            }
        }
    }

  
    return score;    
}
