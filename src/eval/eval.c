#include "eval_internal.h"

/*
PAWN_VAL=100
BISHOP_PAIR=34
ROOK_VAL=541
KNIGHT_VAL=387
QUEEN_VAL=1040
BISHOP_VAL=342
KNIGHT_KAUFMAN_ADJ=4
ROOK_KAUFMAN_ADJ=0
*/
/* material values */
int32_t pawn_val                        =  100;
int32_t knight_val                      =  387;
int32_t bishop_val                      =  342;
int32_t bishop_pair                     =   34;
int32_t rook_val                        =  541;
int32_t queen_val                       = 1040;
int32_t knight_kaufman_adj              =    4;
int32_t rook_kaufman_adj                =    0;

/*
KING_SAFETY_WING_PAWN_FAR_AWAY=-24
KING_SAFETY_PAWN_FAR_AWAY=-23
KING_SAFETY_PAWN_TWO_AWAY=-14
KING_SAFETY_WING_PAWN_TWO_AWAY=-9
KING_SAFETY_WING_PAWN_ONE_AWAY=0
KING_SAFETY_PAWN_ONE_AWAY=-4
KING_SAFETY_MIDDLE_OPEN_FILE=-39
*/
/* king safety */
int32_t king_safety_pawn_one_away       =   -4;
int32_t king_safety_wing_pawn_one_away  =    0;
int32_t king_safety_pawn_two_away       =  -14;
int32_t king_safety_wing_pawn_two_away  =   -9;
int32_t king_safety_pawn_far_away       =  -23;
int32_t king_safety_wing_pawn_far_away  =  -24;
int32_t king_safety_middle_open_file    =  -39;

/*
ISOLATED_PAWN=-13
DOUBLED_PAWN=-7
PASSED_PAWN=31
*/
/* pawn terms */
int32_t passed_pawn                     =   31;
int32_t isolated_pawn                   =  -13;
int32_t doubled_pawn                    =   -7;

/* KNIGHT_TROPISM=-7 */
/* knight terms */
int32_t knight_tropism                  =   -7;

/*
BISHOP_MOBILITY=3
BISHOP_ENDGAME_MOBILITY=1
*/
/* bishop terms */
int32_t bishop_mobility                 =    3;
int32_t bishop_endgame_mobility         =    1;

/*
ROOK_OPEN_FILE=33
ROOK_HALF_OPEN_FILE=20
*/
/* rook terms */
int32_t rook_open_file                  =   33;
int32_t rook_half_open_file             =   20;

/*
QUEEN_MOBILITY=1
QUEEN_ENDGAME_MOBILITY=2
*/
/* queen terms */
int32_t queen_mobility                  =    1;
int32_t queen_endgame_mobility          =    2;

/*
MAJOR_ON_7TH=17
CONNECTED_MAJORS_ON_7TH=64
*/
int32_t major_on_7th                    =   17;
int32_t connected_majors_on_7th         =   64;

/* psts */
int32_t pawn_pst[64] = {
   0,0,0,0,0,0,0,0,103,82,72,66,54,47,43,51,27,48,50,43,40,52,41,27,-17,-14,-12,4,17,21,1,-13,-30,-28,-22,-15,-7,-10,-13,-28,-31,-36,-27,-25,-13,-22,-9,-27,-35,-43,-38,-31,-32,-13,-5,-33,0,0,0,0,0,0,0,0
};

int32_t pawn_endgame_pst[64] = {
   0,0,0,0,0,0,0,0,115,94,81,71,58,48,52,61,74,68,59,47,41,47,47,47,21,13,7,-10,-9,1,5,2,1,-4,-18,-17,-15,-13,-12,-14,-11,-12,-16,-12,-6,-7,-17,-20,0,-6,-5,-19,-8,4,-13,-23,0,0,0,0,0,0,0,0
};

int32_t knight_pst[64] = {
   -41,-30,-30,-30,-30,-30,-30,-31,-38,-35,-24,-20,-24,-30,-36,-36,-34,-16,-1,2,-6,-10,-20,-33,-27,-4,4,12,2,13,-6,-20,-29,-22,-6,-8,-3,-7,-10,-25,-43,-26,-18,-8,-3,-13,-16,-38,-30,-31,-31,-17,-21,-22,-31,-29,-8,-41,-15,-35,-28,-29,-37,-8
};

int32_t knight_endgame_pst[64] = {
   -38,-30,-30,-30,-30,-30,-30,-31,-38,-35,-26,-18,-23,-31,-36,-36,-34,-18,-1,-1,-9,-10,-23,-33,-28,-7,1,9,4,7,-9,-23,-29,-22,-3,-4,-3,-7,-13,-28,-47,-27,-21,-9,-9,-21,-21,-44,-30,-31,-34,-24,-25,-27,-33,-32,-10,-46,-13,-35,-30,-33,-36,-8
};

int32_t bishop_pst[64] = {
   0,0,0,1,0,0,0,0,-12,9,8,7,11,10,7,-1,-2,13,26,26,25,28,21,14,-6,19,21,41,34,27,20,5,-4,10,15,24,29,9,8,3,-8,7,6,12,7,7,6,8,-2,2,7,-8,-6,-2,17,-2,-8,-2,-28,-23,-23,-21,-5,-4
};

int32_t bishop_endgame_pst[64] = {
   0,0,0,2,0,0,0,0,-9,12,8,8,11,10,7,-1,-2,14,25,25,25,30,18,13,-3,21,21,35,34,26,23,5,-4,11,21,25,28,15,8,3,-8,7,12,17,14,9,5,7,-2,-3,3,-3,-1,-4,6,-5,-8,-2,-27,-21,-20,-20,-5,-4
};

int32_t rook_pst[64] = {
   25,23,18,16,17,17,16,24,8,9,15,19,17,14,9,13,8,17,20,19,20,25,16,9,-1,5,11,14,10,16,8,6,-17,-7,-9,-9,-12,-4,0,-6,-31,-19,-23,-24,-23,-18,-2,-16,-36,-27,-27,-30,-28,-14,-13,-22,-19,-18,-16,-12,-13,-9,-7,-24
};

int32_t rook_endgame_pst[64] = {
   28,26,21,19,20,20,19,25,14,16,18,22,20,16,12,15,13,17,20,19,17,25,14,12,4,9,12,14,10,15,8,6,-13,-4,-6,-7,-9,-2,0,-6,-28,-16,-20,-21,-21,-15,-3,-14,-33,-24,-25,-28,-27,-17,-13,-19,-18,-17,-12,-15,-13,-5,-10,-25
};

int32_t queen_pst[64] = {
   -1,7,7,10,16,8,7,10,-30,-45,-5,0,11,19,5,11,-17,-7,1,11,23,38,37,38,-17,-3,0,10,24,30,32,32,-15,-2,0,6,8,19,16,17,-15,-6,0,-5,-4,0,8,-1,-18,-12,-5,-9,-8,-9,-10,-4,-10,-22,-21,-9,-19,-26,-10,-5
};

int32_t queen_endgame_pst[64] = {
   -1,7,10,12,16,10,7,9,-27,-37,-5,1,13,19,5,8,-15,-6,1,12,23,36,34,37,-14,-3,0,12,27,30,32,32,-14,-2,0,11,11,19,16,16,-13,-6,0,-4,-4,1,6,-1,-16,-11,-11,-13,-12,-13,-13,-4,-9,-19,-23,-16,-23,-26,-9,-5
};

int32_t king_pst[64] = {
   -30,-30,-30,-30,-30,-30,-30,-30,-30,-26,-26,-30,-30,-26,-26,-30,-27,-19,-18,-24,-25,-16,-17,-29,-27,-18,-11,-15,-14,-7,-11,-28,-30,-22,-11,-9,-8,-2,-10,-31,-24,-20,-14,-16,-16,-8,-15,-33,-14,-17,-16,-40,-44,-30,-10,-2,-19,-4,-29,-60,-59,-67,-10,-13
};

int32_t king_endgame_pst[64] = {
   -30,-30,-30,-30,-30,-30,-30,-30,-30,-20,-26,-30,-30,-23,-19,-30,-27,-11,-9,-17,-18,-2,-3,-29,-27,-9,1,-3,-3,7,-1,-28,-30,-18,-1,1,3,0,-10,-34,-31,-20,-14,-11,-11,-13,-23,-39,-21,-24,-23,-31,-28,-20,-28,-46,-19,-21,-29,-55,-63,-40,-41,-69
};


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
