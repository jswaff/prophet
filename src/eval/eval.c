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
    /* establish a baseline score using material, from white's perspective. */
    int32_t mat_score = 
        eval_nonpawn_material(pos, true) -     /* white non-pawn material */
        eval_nonpawn_material(pos, false) +    /* black non-pawn material */
        eval_pawn_material(pos, true) -        /* white pawn material */
        eval_pawn_material(pos, false);        /* black pawn material */

    if (material_only)
    {
        return pos->player == WHITE ? mat_score : -mat_score;
    }

    /* evaluate for a draw.  positions that are drawn by rule are immediately 
     * returned.  Others that are "drawish" are further evaluated but later
     * tapered down.
     */
    /*int immediate_draw = 0;
    material_type_t mt = eval_material_type(pos, &immediate_draw);
    if (immediate_draw)
    {
        return 0;
    }*/

    /*int draw_factor = 1;*/
    /*if (KPKN==mt || KPKB==mt || KNKP==mt || KBKP==mt)
    {
        draw_factor = 8;
    }*/

    int32_t mg_score = mat_score;
    /*int32_t eg_score = mat_score;*/

    /* fold in pawn positional features */
    mg_score +=
        eval_accumulator(pos, pos->white_pawns, false, &eval_pawn) -
        eval_accumulator(pos, pos->black_pawns, false, &eval_pawn);
    /*eg_score +=
        eval_accumulator(pos, pos->white_pawns, true, &eval_pawn) -
        eval_accumulator(pos, pos->black_pawns, true, &eval_pawn);
    */

    /* fold in knight positional features */
    mg_score +=
        eval_accumulator(pos, pos->white_knights, false, &eval_knight) -
        eval_accumulator(pos, pos->black_knights, false, &eval_knight);
    /*eg_score +=
        eval_accumulator(pos, pos->white_knights, true, &eval_knight) -
        eval_accumulator(pos, pos->black_knights, true, &eval_knight);*/

    /* fold in bishop positional features */
    mg_score +=
        eval_accumulator(pos, pos->white_bishops, false, &eval_bishop) -
        eval_accumulator(pos, pos->black_bishops, false, &eval_bishop);
    /*eg_score +=
        eval_accumulator(pos, pos->white_bishops, true, &eval_bishop) -
        eval_accumulator(pos, pos->black_bishops, true, &eval_bishop);*/

    /* fold in rook positional features */
    mg_score +=
        eval_accumulator(pos, pos->white_rooks, false, &eval_rook) -
        eval_accumulator(pos, pos->black_rooks, false, &eval_rook);
    /*eg_score +=
        eval_accumulator(pos, pos->white_rooks, true, &eval_rook) -
        eval_accumulator(pos, pos->black_rooks, true, &eval_rook);*/

    /* fold in queen positional features */
    mg_score +=
        eval_accumulator(pos, pos->white_queens, false, &eval_queen) -
        eval_accumulator(pos, pos->black_queens, false, &eval_queen);
    /*eg_score +=
        eval_accumulator(pos, pos->white_queens, true, &eval_queen) -
        eval_accumulator(pos, pos->black_queens, true, &eval_queen);*/

    /*eg_score = mg_score;*/

    /* fold in king positional features.  This includes king safety. */
    mg_score += eval_king(pos, pos->white_king, false) - 
        eval_king(pos, pos->black_king, false);
    /*eg_score += eval_king(pos, pos->white_king, true) - 
        eval_king(pos, pos->black_king, true);*/

    /* calculate a score between [mg_score, eg_score], weighted by the
     * amount of material on the board.  then use the draw factor to pull
     * the score towards a draw. */
    /*int32_t tapered_score = eval_taper(pos, mg_score, eg_score) / draw_factor;*/
    int32_t tapered_score = mg_score;

    /* return the score from the perspective of the player on move */
    return pos->player == WHITE ? tapered_score : -tapered_score;    
}
