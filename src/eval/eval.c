#include <prophet/hash.h>
#include "eval_internal.h"

extern hash_table_t phtbl;

/*
#Error: 0.1165223345282984
#Fri Oct 07 05:37:39 EDT 2022
*/

/* material values */
/*
PAWN_VAL=100
KNIGHT_VAL=373
BISHOP_VAL=362
BISHOP_PAIR=39
ROOK_VAL=557
QUEEN_VAL=1091
KNIGHT_KAUFMAN_ADJ=4
ROOK_KAUFMAN_ADJ=-5
*/
int32_t pawn_val                        =  100;
int32_t knight_val                      =  373;
int32_t bishop_val                      =  362;
int32_t bishop_pair                     =   39;
int32_t rook_val                        =  557;
int32_t queen_val                       = 1091;
int32_t knight_kaufman_adj              =    4;
int32_t rook_kaufman_adj                =   -5;


/* king safety */
/*
KING_SAFETY_PAWN_ONE_AWAY=-7
KING_SAFETY_WING_PAWN_ONE_AWAY=9
KING_SAFETY_PAWN_TWO_AWAY=-18
KING_SAFETY_WING_PAWN_TWO_AWAY=6
KING_SAFETY_PAWN_FAR_AWAY=-28
KING_SAFETY_WING_PAWN_FAR_AWAY=-18
KING_SAFETY_MIDDLE_OPEN_FILE=-54
*/
int32_t king_safety_pawn_one_away       =   -7;
int32_t king_safety_wing_pawn_one_away  =    9;
int32_t king_safety_pawn_two_away       =  -18;
int32_t king_safety_wing_pawn_two_away  =    6;
int32_t king_safety_pawn_far_away       =  -28;
int32_t king_safety_wing_pawn_far_away  =  -18;
int32_t king_safety_middle_open_file    =  -54;


/* pawn terms */
/*
PASSED_PAWN_MG=0,84,47,17,-14,-20,-10,0
PASSED_PAWN_EG=0,155,112,64,47,25,19,0
ISOLATED_PAWN_MG=-11
ISOLATED_PAWN_EG=-14
DOUBLED_PAWN_MG=-7
DOUBLED_PAWN_EG=-8
*/
int32_t passed_pawn_mg[8]               =   { 0,84,47,17,-14,-20,-10,0 };
int32_t passed_pawn_eg[8]               =   { 0,155,112,64,47,25,19,0 };
int32_t isolated_pawn_mg                =  -11;
int32_t isolated_pawn_eg                =  -14;
int32_t doubled_pawn_mg                 =   -7;
int32_t doubled_pawn_eg                 =   -8;


/* knight terms */
/*
KNIGHT_TROPISM_MG=-7
KNIGHT_TROPISM_EG=-6
KNIGHT_OUTPOST=-19,-4,-4,-4,-1,-4,-2,-7,-14,-8,4,13,9,2,-8,-9,-9,7,21,21,24,24,11,-5,2,10,17,25,24,25,19,8,2,8,11,16,15,14,7,7,-12,4,5,2,3,2,0,-4,-12,-3,-3,-1,-1,-4,-3,-13,-6,-10,-6,-4,-3,-6,-2,-3
KNIGHT_SUPPORTED_OUTPOST=0,0,0,0,0,0,0,0,0,1,1,1,2,0,0,0,0,10,16,17,13,16,10,1,4,16,17,20,19,19,17,6,10,7,19,19,8,12,6,4,-4,-1,-5,-1,-2,-6,-2,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
*/
int32_t knight_tropism_mg               =   -7;
int32_t knight_tropism_eg               =   -6;
int32_t knight_outpost[64]              =    { -19,-4,-4,-4,-1,-4,-2,-7,-14,-8,4,13,9,2,-8,-9,-9,7,21,21,24,24,11,-5,2,10,17,25,24,25,19,8,2,8,11,16,15,14,7,7,-12,4,5,2,3,2,0,-4,-12,-3,-3,-1,-1,-4,-3,-13,-6,-10,-6,-4,-3,-6,-2,-3 };
int32_t knight_supported_outpost[64]    =    { 0,0,0,0,0,0,0,0,0,1,1,1,2,0,0,0,0,10,16,17,13,16,10,1,4,16,17,20,19,19,17,6,10,7,19,19,8,12,6,4,-4,-1,-5,-1,-2,-6,-2,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };


/* bishop terms */
/*
BISHOP_MOBILITY_MG=-25,-19,-13,-10,-4,1,4,3,5,9,17,20,6,7
BISHOP_MOBILITY_EG=-20,-36,-30,-16,0,9,18,23,28,25,21,21,8,11
BISHOP_TRAPPED=-125
*/
int32_t bishop_mobility_mg[14]          =    { -25,-19,-13,-10,-4,1,4,3,5,9,17,20,6,7 };
int32_t bishop_mobility_eg[14]          =    { -20,-36,-30,-16,0,9,18,23,28,25,21,21,8,11 };
int32_t bishop_trapped                  =   -125;


/* rook terms */
/*
ROOK_MOBILITY_MG=-35,-31,-27,-24,-27,-21,-20,-16,-11,-9,-4,-3,6,16,14
ROOK_MOBILITY_EG=-20,-22,-14,-2,14,19,25,27,31,36,36,37,33,25,24
ROOK_OPEN_FILE_MG=27
ROOK_OPEN_FILE_SUPPORTED_MG=11
ROOK_OPEN_FILE_EG=18
ROOK_OPEN_FILE_SUPPORTED_EG=3
ROOK_HALF_OPEN_FILE_MG=15
ROOK_HALF_OPEN_FILE_SUPPORTED_MG=10
ROOK_HALF_OPEN_FILE_EG=12
ROOK_HALF_OPEN_FILE_SUPPORTED_EG=9
*/
int32_t rook_mobility_mg[15]            =    { -35,-31,-27,-24,-27,-21,-20,-16,-11,-9,-4,-3,6,16,14 };
int32_t rook_mobility_eg[15]            =    { -20,-22,-14,-2,14,19,25,27,31,36,36,37,33,25,24 };
int32_t rook_open_file_mg               =   27;
int32_t rook_open_file_supported_mg     =   11;
int32_t rook_open_file_eg               =   18;
int32_t rook_open_file_supported_eg     =    3;
int32_t rook_half_open_file_mg          =   15;
int32_t rook_half_open_file_supported_mg =  10;
int32_t rook_half_open_file_eg          =   12;
int32_t rook_half_open_file_supported_eg =   9;


/* queen terms */
/*
QUEEN_MOBILITY_MG=-2,-7,-11,-11,-10,-9,-7,-6,-3,1,2,4,6,7,8,11,12,12,15,14,12,11,6,4,2,1,0,0
QUEEN_MOBILITY_EG=0,-1,-3,-5,-9,-14,-14,-11,-7,-3,1,7,10,13,18,17,18,18,20,20,17,16,9,7,3,2,1,1
*/
int32_t queen_mobility_mg[28]           =    { -2,-7,-11,-11,-10,-9,-7,-6,-3,1,2,4,6,7,8,11,12,12,15,14,12,11,6,4,2,1,0,0 };
int32_t queen_mobility_eg[28]           =    { 0,-1,-3,-5,-9,-14,-14,-11,-7,-3,1,7,10,13,18,17,18,18,20,20,17,16,9,7,3,2,1,1 };

/*
MAJOR_ON_7TH_MG=-16
MAJOR_ON_7TH_EG=51
CONNECTED_MAJORS_ON_7TH_MG=11
CONNECTED_MAJORS_ON_7TH_EG=16
*/
int32_t major_on_7th_mg                 =  -16;
int32_t major_on_7th_eg                 =   51;
int32_t connected_majors_on_7th_mg      =   11;
int32_t connected_majors_on_7th_eg      =   16;


/*
PAWN_PST_MG=0,0,0,0,0,0,0,0,26,20,16,13,7,5,-2,0,-14,6,13,5,13,39,11,3,-24,-14,-14,6,17,25,6,-11,-30,-25,-17,-9,-2,-4,-4,-31,-28,-32,-22,-16,-6,-13,-1,-24,-32,-35,-37,-29,-29,-12,3,-22,0,0,0,0,0,0,0,0
PAWN_PST_EG=0,0,0,0,0,0,0,0,50,38,24,11,6,4,8,14,55,40,20,-6,-9,13,15,18,29,20,8,-16,-9,-6,7,4,12,10,-7,-9,-9,-5,-5,-6,4,0,-4,-6,2,1,-9,-11,12,3,7,0,12,9,-8,-16,0,0,0,0,0,0,0,0
KNIGHT_PST_MG=-12,-1,-3,-2,0,-3,0,-4,-7,-6,4,3,3,6,-3,-2,-4,3,6,11,15,10,7,1,0,5,13,11,11,36,14,8,-10,1,9,6,20,12,21,-8,-22,-4,6,18,25,12,7,-20,-16,-11,-7,8,5,5,-14,-10,-4,-15,-14,-8,-4,-1,-13,-6
KNIGHT_PST_EG=-7,-2,-1,-2,-1,-1,-2,-4,-8,-2,0,10,6,-3,-5,-7,-5,4,10,7,5,9,2,-6,-4,3,12,12,18,11,6,-5,-8,5,18,18,23,21,9,-7,-17,-3,3,18,15,2,-3,-20,-8,-7,-8,-1,1,-9,-9,-10,-6,-24,-10,-7,-9,-13,-16,-4
BISHOP_PST_MG=-5,0,-3,-2,-2,-4,0,-1,-14,-7,-5,-4,-3,2,-5,-4,-11,0,11,7,11,10,12,7,-17,8,3,33,19,13,9,-1,-10,-2,0,14,21,-7,-3,1,-12,2,-2,4,-2,3,2,6,-2,5,4,-11,-5,-2,22,-3,-1,1,-18,-20,-22,-13,-5,-4
BISHOP_PST_EG=0,3,1,3,3,-1,0,0,-5,4,2,2,3,4,0,-2,-2,8,8,9,10,17,10,4,-4,8,8,18,21,10,12,2,-6,3,13,16,12,10,-1,-3,-5,1,8,7,9,-1,-4,-1,-5,-11,-8,-6,-11,-14,-11,-12,-8,-3,-14,-13,-9,-15,-6,-4
ROOK_PST_MG=9,10,4,4,6,8,9,13,0,-3,10,13,12,11,6,12,-6,11,9,16,19,22,19,11,-13,-2,2,11,6,15,10,9,-25,-14,-19,-16,-16,-5,3,-5,-31,-22,-23,-21,-22,-14,4,-11,-33,-24,-26,-27,-24,-7,-8,-21,-16,-15,-9,0,-3,2,2,-12
ROOK_PST_EG=28,30,28,24,25,26,25,29,14,16,16,20,18,13,12,12,19,18,23,17,15,24,15,15,12,13,17,15,12,15,10,11,-4,4,5,1,0,4,5,-2,-18,-10,-15,-15,-13,-10,-5,-10,-23,-21,-19,-21,-21,-20,-14,-12,-13,-11,-9,-17,-15,-5,-10,-22
QUEEN_PST_MG=-2,4,6,7,13,9,8,13,-21,-42,-6,1,10,19,7,18,-17,-10,-8,5,19,37,43,45,-16,-10,-7,-2,16,23,33,35,-17,-9,-11,-5,-1,12,12,18,-17,-8,-3,-10,-8,-1,7,1,-18,-10,-1,-5,-2,-2,-5,-3,-13,-18,-9,3,-8,-25,-7,-5
QUEEN_PST_EG=1,6,10,13,17,11,8,11,-8,-2,1,7,16,17,7,10,-7,-3,2,10,20,28,24,24,-10,2,1,14,27,24,24,21,-8,-1,1,17,15,16,9,11,-8,-6,0,-4,-5,2,-2,-1,-10,-11,-27,-17,-23,-22,-15,-4,-9,-12,-16,-23,-18,-17,-6,-3
KING_PST_MG=0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,0,0,3,4,2,2,5,5,-1,0,3,6,4,4,7,5,-2,-2,2,7,3,5,7,7,-7,-3,0,4,0,2,4,10,-9,1,0,-3,-30,-31,-22,19,29,-12,26,2,-38,-25,-53,27,30
KING_PST_EG=-3,-2,-1,-1,-2,-1,-1,-2,-2,5,4,1,1,5,8,-3,1,15,18,9,7,23,24,-3,-2,13,26,23,23,30,22,-7,-10,4,22,29,28,23,9,-19,-12,-2,7,15,13,9,-5,-22,-8,-8,-3,-7,-2,3,-14,-36,-14,-19,-16,-31,-44,-13,-31,-73
*/

/* psts */
int32_t pawn_pst_mg[64] = {
    0,0,0,0,0,0,0,0,26,20,16,13,7,5,-2,0,-14,6,13,5,13,39,11,3,-24,-14,-14,6,17,25,6,-11,-30,-25,-17,-9,-2,-4,-4,-31,-28,-32,-22,-16,-6,-13,-1,-24,-32,-35,-37,-29,-29,-12,3,-22,0,0,0,0,0,0,0,0
};

int32_t pawn_pst_eg[64] = {
    0,0,0,0,0,0,0,0,50,38,24,11,6,4,8,14,55,40,20,-6,-9,13,15,18,29,20,8,-16,-9,-6,7,4,12,10,-7,-9,-9,-5,-5,-6,4,0,-4,-6,2,1,-9,-11,12,3,7,0,12,9,-8,-16,0,0,0,0,0,0,0,0
};

int32_t knight_pst_mg[64] = {
    -12,-1,-3,-2,0,-3,0,-4,-7,-6,4,3,3,6,-3,-2,-4,3,6,11,15,10,7,1,0,5,13,11,11,36,14,8,-10,1,9,6,20,12,21,-8,-22,-4,6,18,25,12,7,-20,-16,-11,-7,8,5,5,-14,-10,-4,-15,-14,-8,-4,-1,-13,-6
};

int32_t knight_pst_eg[64] = {
    -7,-2,-1,-2,-1,-1,-2,-4,-8,-2,0,10,6,-3,-5,-7,-5,4,10,7,5,9,2,-6,-4,3,12,12,18,11,6,-5,-8,5,18,18,23,21,9,-7,-17,-3,3,18,15,2,-3,-20,-8,-7,-8,-1,1,-9,-9,-10,-6,-24,-10,-7,-9,-13,-16,-4
};

int32_t bishop_pst_mg[64] = {
    -5,0,-3,-2,-2,-4,0,-1,-14,-7,-5,-4,-3,2,-5,-4,-11,0,11,7,11,10,12,7,-17,8,3,33,19,13,9,-1,-10,-2,0,14,21,-7,-3,1,-12,2,-2,4,-2,3,2,6,-2,5,4,-11,-5,-2,22,-3,-1,1,-18,-20,-22,-13,-5,-4
};

int32_t bishop_pst_eg[64] = {
    0,3,1,3,3,-1,0,0,-5,4,2,2,3,4,0,-2,-2,8,8,9,10,17,10,4,-4,8,8,18,21,10,12,2,-6,3,13,16,12,10,-1,-3,-5,1,8,7,9,-1,-4,-1,-5,-11,-8,-6,-11,-14,-11,-12,-8,-3,-14,-13,-9,-15,-6,-4
};

int32_t rook_pst_mg[64] = {
    9,10,4,4,6,8,9,13,0,-3,10,13,12,11,6,12,-6,11,9,16,19,22,19,11,-13,-2,2,11,6,15,10,9,-25,-14,-19,-16,-16,-5,3,-5,-31,-22,-23,-21,-22,-14,4,-11,-33,-24,-26,-27,-24,-7,-8,-21,-16,-15,-9,0,-3,2,2,-12
};

int32_t rook_pst_eg[64] = {
    28,30,28,24,25,26,25,29,14,16,16,20,18,13,12,12,19,18,23,17,15,24,15,15,12,13,17,15,12,15,10,11,-4,4,5,1,0,4,5,-2,-18,-10,-15,-15,-13,-10,-5,-10,-23,-21,-19,-21,-21,-20,-14,-12,-13,-11,-9,-17,-15,-5,-10,-22
};

int32_t queen_pst_mg[64] = {
    -2,4,6,7,13,9,8,13,-21,-42,-6,1,10,19,7,18,-17,-10,-8,5,19,37,43,45,-16,-10,-7,-2,16,23,33,35,-17,-9,-11,-5,-1,12,12,18,-17,-8,-3,-10,-8,-1,7,1,-18,-10,-1,-5,-2,-2,-5,-3,-13,-18,-9,3,-8,-25,-7,-5
};

int32_t queen_pst_eg[64] = {
    1,6,10,13,17,11,8,11,-8,-2,1,7,16,17,7,10,-7,-3,2,10,20,28,24,24,-10,2,1,14,27,24,24,21,-8,-1,1,17,15,16,9,11,-8,-6,0,-4,-5,2,-2,-1,-10,-11,-27,-17,-23,-22,-15,-4,-9,-12,-16,-23,-18,-17,-6,-3
};

int32_t king_pst_mg[64] = {
    0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,0,0,3,4,2,2,5,5,-1,0,3,6,4,4,7,5,-2,-2,2,7,3,5,7,7,-7,-3,0,4,0,2,4,10,-9,1,0,-3,-30,-31,-22,19,29,-12,26,2,-38,-25,-53,27,30
};

int32_t king_pst_eg[64] = {
    -3,-2,-1,-1,-2,-1,-1,-2,-2,5,4,1,1,5,8,-3,1,15,18,9,7,23,24,-3,-2,13,26,23,23,30,22,-7,-10,4,22,29,28,23,9,-19,-12,-2,7,15,13,9,-5,-22,-8,-8,-3,-7,-2,3,-14,-36,-14,-19,-16,-31,-44,-13,-31,-73
};

#ifndef NDEBUG
static bool verify_pawn_scores(const position_t* pos, int32_t mg_score, int32_t eg_score);
#endif

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

    /* fold in pawn positional features. try the pawn hash first. */
    uint64_t pawn_hash_val = probe_hash(&phtbl, pos->pawn_key);
    if (pawn_hash_val != 0)
    {
        mg_score += get_pawn_hash_entry_mg_score(pawn_hash_val);
        eg_score += get_pawn_hash_entry_eg_score(pawn_hash_val);
        assert(verify_pawn_scores(pos, mg_score - mat_score, eg_score - mat_score));
    }
    else
    {
        uint64_t all_pawns = pos->white_pawns | pos->black_pawns;
        while (all_pawns)
        {
            square_t sq = (square_t)get_lsb(all_pawns);
            eval_pawn(pos, sq, &mg_score, &eg_score);
            all_pawns ^= square_to_bitmap(sq);
        }
        /* store the scores */
        store_hash_entry(&phtbl, pos->pawn_key, 
            build_pawn_hash_val(mg_score - mat_score, eg_score - mat_score));
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

#ifndef NDEBUG
static bool verify_pawn_scores(const position_t* pos, int32_t mg_score, int32_t eg_score)
{
    int32_t my_mg_score = 0;
    int32_t my_eg_score = 0;
    uint64_t all_pawns = pos->white_pawns | pos->black_pawns;
    while (all_pawns)
    {
        square_t sq = (square_t)get_lsb(all_pawns);
        eval_pawn(pos, sq, &my_mg_score, &my_eg_score);
        all_pawns ^= square_to_bitmap(sq);
    }

    assert(mg_score==my_mg_score);
    assert(eg_score==my_eg_score);

    return mg_score==my_mg_score && eg_score==my_eg_score;
}
#endif