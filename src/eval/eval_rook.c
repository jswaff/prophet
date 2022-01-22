#include <prophet/bitmap.h>
#include <prophet/parameters.h>

#include "eval_internal.h"

#include  <assert.h>

/* forward decls */
static int32_t eval_rook_open_file(const position_t* pos, square_t sq);


/**
 * \brief Evaluate a single rook.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the rook is on
 * \param endgame       if the eval should be done in the endgame phase 
 *
 * \return a score for the rook.
 */
int32_t eval_rook(const position_t* pos, square_t sq, bool UNUSED(endgame))
{
    assert(pos->piece[sq] == ROOK || pos->piece[sq] == -ROOK);

    int32_t score = 0; 

    if (is_white_piece(pos->piece[sq]))
    {
        score = rook_pst[sq];
    }
    else
    {
        score = rook_pst[flip_rank[sq]];
    }

    score += eval_major_on_7th(pos, sq);
    score += eval_rook_open_file(pos, sq);
  
    return score;    
}


/**
 * \brief Evaluate a square for an open or half open file.
 *
 * An open file is defined as a file with no pawns.  A half open file is 
 * defined as a file with no friendly pawns, but at least one enemy pawn.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the rook is on
 *
 * \return a bonus for an open or half open file.
 */ 
static int32_t eval_rook_open_file(const position_t* pos, square_t sq)
{
    int32_t score = 0;

    uint64_t friends, enemies;
    if (is_white_piece(pos->piece[sq]))
    {
        friends = pos->white_pawns;
        enemies = pos->black_pawns;
    }
    else
    {
        friends = pos->black_pawns;
        enemies = pos->white_pawns;
    }

    uint64_t file_mask = sq_to_file_bitmap(sq);
    if (!(file_mask & friends))
    {
        if (file_mask & enemies)
        {
            score = rook_half_open_file;
        }
        else
        {
            score = rook_open_file;
        }
    }

    return score;
}
