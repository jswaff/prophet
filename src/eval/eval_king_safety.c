#include <prophet/bitmap.h>

#include "eval_internal.h"


/**
 * \brief Evaluate king safety for one side of the board.
 *
 * \param pos           a pointer to a chess position
 * \param wtm           white to move?
 *
 * \return a score of the king safety for the player specified.
 */
int32_t eval_king_safety(const position_t* pos, bool wtm)
{
    int32_t score = 0;

    if (wtm)
    {
        file_t file_king = get_file(pos->white_king);

        /* king side */
        if (file_king > FILE_E)
        {
            /* test the F pawn */
            if (pos->piece[F2] != PAWN)
            {
                if (pos->piece[F3] == PAWN)
                {
                    score += king_safety_pawn_one_away;
                }
                else if (pos->piece[F4] == PAWN)
                {
                    score += king_safety_pawn_two_away;
                }
                else 
                {
                    score += king_safety_pawn_far_away;
                }
            }

            /* test the G pawn */
            if (pos->piece[G2] != PAWN)
            {
                if (pos->piece[G3] == PAWN)
                {
                    score += king_safety_pawn_one_away;
                }
                else if (pos->piece[G4] == PAWN)
                {
                    score += king_safety_pawn_two_away;
                }
                else
                {
                    score += king_safety_pawn_far_away;
                }
            }

            /* test the H pawn */
            if (pos->piece[H2] != PAWN)
            {
                if (pos->piece[H3] == PAWN)
                {
                    score += king_safety_pawn_one_away / 2;
                }
                else if (pos->piece[H4] == PAWN)
                {
                    score += king_safety_pawn_two_away / 2;
                }
                else
                {
                    score += king_safety_pawn_far_away / 2;
                }
            }
        }
        /* queen side */
        else if (file_king < FILE_D)
        {
            /* test the C pawn */
            if (pos->piece[C2] != PAWN)
            {
                if (pos->piece[C3] == PAWN)
                {
                    score += king_safety_pawn_one_away;
                }
                else if (pos->piece[C4] == PAWN)
                {
                    score += king_safety_pawn_two_away;
                }
                else 
                {
                    score += king_safety_pawn_far_away;
                }
            }

            /* test the B pawn */
            if (pos->piece[B2] != PAWN)
            {
                if (pos->piece[B3] == PAWN)
                {
                    score += king_safety_pawn_one_away;
                }
                else if (pos->piece[B4] == PAWN)
                {
                    score += king_safety_pawn_two_away;
                }
                else 
                {
                    score += king_safety_pawn_far_away;
                }
            }

            /* test the A pawn */
            if (pos->piece[A2] != PAWN)
            {
                if (pos->piece[A3] == PAWN)
                {
                    score += king_safety_pawn_one_away / 2;
                }
                else if (pos->piece[A4] == PAWN)
                {
                    score += king_safety_pawn_two_away / 2;
                }
                else 
                {
                    score += king_safety_pawn_far_away / 2;
                }
            }
        }
        /* middle */
        else
        {
            if (! ((pos->white_pawns | pos->black_pawns) & 
                sq_to_file_bitmap(pos->white_king))) 
            {
                score += king_safety_middle_open_file;
            }
        }
    }
    else /* black to move */
    {
        file_t file_king = get_file(pos->black_king);

        /* king side */
        if (file_king > FILE_E)
        {
            /* test the F pawn */
            if (pos->piece[F7] != -PAWN)
            {
                if (pos->piece[F6] == -PAWN)
                {
                    score += king_safety_pawn_one_away;
                }
                else if (pos->piece[F5] == -PAWN)
                {
                    score += king_safety_pawn_two_away;
                }
                else 
                {
                    score += king_safety_pawn_far_away;
                }
            }

            /* test the G pawn */
            if (pos->piece[G7] != -PAWN)
            {
                if (pos->piece[G6] == -PAWN)
                {
                    score += king_safety_pawn_one_away;
                }
                else if (pos->piece[G5] == -PAWN)
                {
                    score += king_safety_pawn_two_away;
                }
                else
                {
                    score += king_safety_pawn_far_away;
                }
            }

            /* test the H pawn */
            if (pos->piece[H7] != -PAWN)
            {
                if (pos->piece[H6] == -PAWN)
                {
                    score += king_safety_pawn_one_away / 2;
                }
                else if (pos->piece[H5] == -PAWN)
                {
                    score += king_safety_pawn_two_away / 2;
                }
                else
                {
                    score += king_safety_pawn_far_away / 2;
                }
            }
        }
        /* queen side */
        else if (file_king < FILE_D)
        {
            /* test the C pawn */
            if (pos->piece[C7] != -PAWN)
            {
                if (pos->piece[C6] == -PAWN)
                {
                    score += king_safety_pawn_one_away;
                }
                else if (pos->piece[C5] == -PAWN)
                {
                    score += king_safety_pawn_two_away;
                }
                else 
                {
                    score += king_safety_pawn_far_away;
                }
            }

            /* test the B pawn */
            if (pos->piece[B7] != -PAWN)
            {
                if (pos->piece[B6] == -PAWN)
                {
                    score += king_safety_pawn_one_away;
                }
                else if (pos->piece[B5] == -PAWN)
                {
                    score += king_safety_pawn_two_away;
                }
                else 
                {
                    score += king_safety_pawn_far_away;
                }
            }

            /* test the A pawn */
            if (pos->piece[A7] != -PAWN)
            {
                if (pos->piece[A6] == -PAWN)
                {
                    score += king_safety_pawn_one_away / 2;
                }
                else if (pos->piece[A5] == -PAWN)
                {
                    score += king_safety_pawn_two_away / 2;
                }
                else 
                {
                    score += king_safety_pawn_far_away / 2;
                }
            }
        }
        /* middle */
        else
        {
            if (! ((pos->white_pawns | pos->black_pawns) & 
                sq_to_file_bitmap(pos->black_king))) 
            {
                score += king_safety_middle_open_file;
            }
        }
    }

    return score;
}