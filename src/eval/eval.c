#include "eval_internal.h"

/**
 * \brief Evaluate a chess position for the side to move.
 *
 * Performs a static analysis of a chess position.  The score is primarily
 * influenced by material counts, but it also takes into account several
 * well known heuristics, such as king safety, passed pawns, isolated
 * pawns, rooks on open files, and several others.  
 *
 * This method will not detect end-of-game scenarios such as checkmate.
 *
 * \param pos             a pointer to a chess position
 * \param material_only   if the evaluation should consider material only
 *
 * \return the score.
 */
int32_t eval(const position_t* pos, bool material_only)
{
    if (eval_draw(pos))
    {
        return 0;
    }

    /* establish a baseline score using material, from white's perspective. */
    int32_t mg_score = 
        eval_nonpawn_material(pos, true) -     /* white non-pawn material */
        eval_nonpawn_material(pos, false) +    /* black non-pawn material */
        eval_pawn_material(pos, true) -        /* white pawn material */
        eval_pawn_material(pos, false);        /* black pawn material */
    int32_t eg_score = mg_score;

    if (!material_only)
    {
        /* fold in pawn positional features */
        mg_score +=
            eval_accumulator(pos, pos->white_pawns, &eval_pawn) -
            eval_accumulator(pos, pos->black_pawns, &eval_pawn);

        /* fold in knight positional features */
        mg_score +=
            eval_accumulator(pos, pos->white_knights, &eval_knight) -
            eval_accumulator(pos, pos->black_knights, &eval_knight);

        /* fold in bishop positional features */
        mg_score +=
            eval_accumulator(pos, pos->white_bishops, &eval_bishop) -
            eval_accumulator(pos, pos->black_bishops, &eval_bishop);
    
        /* fold in rook positional features */
        mg_score +=
            eval_accumulator(pos, pos->white_rooks, &eval_rook) -
            eval_accumulator(pos, pos->black_rooks, &eval_rook);

        /* fold in queen positional features */
        mg_score +=
            eval_accumulator(pos, pos->white_queens, &eval_queen) -
            eval_accumulator(pos, pos->black_queens, &eval_queen);

        /* fold in king positional features.  This includes king safety. */
        eg_score = mg_score;
        mg_score += eval_king(pos, pos->white_king, false) - 
            eval_king(pos, pos->black_king, false);
        eg_score += eval_king(pos, pos->white_king, true) - 
            eval_king(pos, pos->black_king, true);
    }

    int32_t phase = eval_phase(pos);

    phase = (phase * 256 + 12) / 24;

    int32_t score = (mg_score * (256 - phase) + eg_score * phase) / 256;

    /* return the score from the perspective of the player on move */
    return pos->player == WHITE ? score : -score;    
}
