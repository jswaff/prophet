#include "eval_internal.h"

/* material values */
int32_t pawn_val                        =  100;
int32_t knight_val                      =  325;
int32_t bishop_val                      =  342;
int32_t bishop_pair                     =   34;
int32_t rook_val                        =  541;
int32_t queen_val                       = 1040;
int32_t knight_kaufman_adj              =    4;
int32_t rook_kaufman_adj                =    0;

/* king safety */
int32_t king_safety_pawn_one_away       =   -4;
int32_t king_safety_wing_pawn_one_away  =    0;
int32_t king_safety_pawn_two_away       =  -14;
int32_t king_safety_wing_pawn_two_away  =   -9;
int32_t king_safety_pawn_far_away       =  -23;
int32_t king_safety_wing_pawn_far_away  =  -24;
int32_t king_safety_middle_open_file    =  -39;

/* pawn terms */
int32_t passed_pawn                     =   31;
int32_t isolated_pawn                   =  -13;
int32_t doubled_pawn                    =   -7;

/* knight terms */
int32_t knight_tropism                  =   -7;

/* bishop terms */
int32_t bishop_mobility                 =    3;
int32_t bishop_endgame_mobility         =    1;

/* rook terms */
int32_t rook_open_file                  =   33;
int32_t rook_half_open_file             =   20;

/* queen terms */
int32_t queen_mobility                  =    1;
int32_t queen_endgame_mobility          =    2;

int32_t major_on_7th                    =   17;
int32_t connected_majors_on_7th         =   64;

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

    /* fold in pawn positional features */
    uint64_t all_pawns = pos->white_pawns | pos->black_pawns;
    while (all_pawns)
    {
        square_t sq = (square_t)get_lsb(all_pawns);
        eval_pawn(pos, sq, &mg_score, &eg_score);
        all_pawns ^= square_to_bitmap(sq);
    }

    /* fold in knight positional features */
    uint64_t all_knights = pos->white_knights | pos->black_knights;
    while (all_knights)
    {
        square_t sq = (square_t)get_lsb(all_knights);
        eval_knight(pos, sq, &mg_score, &eg_score);
        all_knights ^= square_to_bitmap(sq);
    }    

    /* fold in bishop positional features */
    uint64_t all_bishops = pos->white_bishops | pos->black_bishops;
    while (all_bishops)
    {
        square_t sq = (square_t)get_lsb(all_bishops);
        eval_bishop(pos, sq, &mg_score, &eg_score);
        all_bishops ^= square_to_bitmap(sq);
    }

    /* fold in rook positional features */
    uint64_t all_rooks = pos->white_rooks | pos->black_rooks;
    while (all_rooks)
    {
        square_t sq = (square_t)get_lsb(all_rooks);
        eval_rook(pos, sq, &mg_score, &eg_score);
        all_rooks ^= square_to_bitmap(sq);
    }

    /* fold in queen positional features */
    uint64_t all_queens = pos->white_queens | pos->black_queens;
    while (all_queens)
    {
        square_t sq = (square_t)get_lsb(all_queens);
        eval_queen(pos, sq, &mg_score, &eg_score);
        all_queens ^= square_to_bitmap(sq);
    }


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
