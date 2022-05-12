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
    int immediate_draw = 0;
    material_type_t mt = eval_material_type(pos, &immediate_draw);
    if (immediate_draw)
    {
        return 0;
    }

    int draw_factor = 1;
    if (KPKN==mt || KPKB==mt || KNKP==mt || KBKP==mt)
    {
        draw_factor = 8;
    }

    int32_t mg_score = mat_score;
    int32_t eg_score = mat_score;


    /* fold in knight positional features */
    eval_accumulator(pos, pos->white_knights | pos->black_knights, &mg_score, &eg_score, &eval_knight);

    /* fold in bishop positional features */
    eval_accumulator(pos, pos->white_bishops | pos->black_bishops, &mg_score, &eg_score, &eval_bishop);

    /* fold in rook positional features */
    eval_accumulator(pos, pos->white_rooks | pos->black_rooks, &mg_score, &eg_score, &eval_rook);

    /* fold in queen positional features */
    eval_accumulator(pos, pos->white_queens | pos->black_queens, &mg_score, &eg_score, &eval_queen);

    /* fold in pawn positional features */
    eval_accumulator(pos, pos->white_pawns | pos->black_pawns, &mg_score, &eg_score, &eval_pawn);

    /* fold in king positional features.  This includes king safety. */
    eval_king(pos, pos->white_king, &mg_score, &eg_score);
    eval_king(pos, pos->black_king, &mg_score, &eg_score);


    /* calculate a score between [mg_score, eg_score], weighted by the
     * amount of material on the board.  then use the draw factor to pull
     * the score towards a draw. */
    int32_t tapered_score = eval_taper(pos, mg_score, eg_score) / draw_factor;


    /* return the score from the perspective of the player on move */
    return pos->player == WHITE ? tapered_score : -tapered_score;    
}
