#include <prophet/hash.h>
#include "eval_internal.h"

extern hash_table_t phtbl;


/* material values */
int32_t pawn_val                        =  100;
int32_t knight_val                      =  372;
int32_t bishop_val                      =  362;
int32_t bishop_pair                     =   39;
int32_t rook_val                        =  557;
int32_t queen_val                       = 1090;
int32_t knight_kaufman_adj              =    4;
int32_t rook_kaufman_adj                =   -5;

/* king safety */
int32_t king_safety_pawn_one_away       =   -7;
int32_t king_safety_wing_pawn_one_away  =    8;
int32_t king_safety_pawn_two_away       =  -18;
int32_t king_safety_wing_pawn_two_away  =    6;
int32_t king_safety_pawn_far_away       =  -28;
int32_t king_safety_wing_pawn_far_away  =  -18;
int32_t king_safety_middle_open_file    =  -54;

/* pawn terms */
int32_t passed_pawn_mg[8]               =   { 0,86,46,17,-14,-19,-10,0 };
int32_t passed_pawn_eg[8]               =   { 0,156,112,64,48,25,19,0 };
int32_t isolated_pawn_mg                =  -11;
int32_t isolated_pawn_eg                =  -14;
int32_t doubled_pawn_mg                 =   -7;
int32_t doubled_pawn_eg                 =   -8;


/* knight terms */
int32_t knight_tropism_mg               =   -7;
int32_t knight_tropism_eg               =   -5;
int32_t knight_outpost[64]              =    { -20,-4,-3,-5,-1,-5,-2,-8,-14,-9,4,13,9,2,-8,-9,-9,7,21,21,23,25,11,-5,2,10,18,25,24,25,19,9,2,9,12,16,15,14,7,7,-12,5,6,3,3,3,0,-5,-11,-3,-5,-1,0,-4,-2,-13,-6,-11,-6,-4,-4,-6,-2,-4 };
int32_t knight_supported_outpost[64]    =    { 0,0,0,0,0,0,0,0,0,1,1,1,2,0,0,0,1,10,16,17,13,17,10,1,4,17,18,20,20,18,18,6,10,6,19,19,9,11,5,5,-4,0,-5,0,-2,-6,-1,-2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };


/* bishop terms */
int32_t bishop_mobility_mg[14]          =    { -24,-19,-13,-10,-5,1,3,3,6,9,17,20,5,7 };
int32_t bishop_mobility_eg[14]          =    { -19,-37,-30,-16,-1,9,17,23,27,26,21,21,8,11 };
int32_t bishop_trapped                  =   -100;

/* rook terms */
int32_t rook_mobility_mg[15]            =    { -35,-31,-25,-24,-26,-21,-19,-15,-11,-10,-5,-4,7,16,12 };
int32_t rook_mobility_eg[15]            =    { -21,-20,-15,-2,14,19,26,27,31,36,36,38,31,25,24 };
int32_t rook_open_file_mg               =   29;
int32_t rook_open_file_eg               =   19;
int32_t rook_half_open_file_mg          =   17;
int32_t rook_half_open_file_eg          =   13;


/* queen terms */
int32_t queen_mobility_mg[28]           =    { -3,-8,-13,-11,-10,-9,-7,-6,-3,0,3,5,5,7,8,11,12,14,15,14,13,11,6,4,2,1,0,0 };
int32_t queen_mobility_eg[28]           =    { 0,-1,-3,-5,-9,-14,-15,-11,-6,-3,1,6,10,14,18,17,18,18,20,20,17,17,8,7,4,2,0,0 };

int32_t major_on_7th_mg                 =  -16;
int32_t major_on_7th_eg                 =   52;
int32_t connected_majors_on_7th_mg      =    9;
int32_t connected_majors_on_7th_eg      =   12;


/* psts */
int32_t pawn_pst_mg[64] = {
    0,0,0,0,0,0,0,0,26,20,16,14,8,4,-1,0,-13,6,13,5,13,40,12,3,-24,-15,-13,6,16,25,5,-10,-29,-25,-17,-8,-3,-3,-4,-32,-28,-31,-22,-17,-6,-13,-2,-24,-31,-36,-38,-31,-29,-11,2,-22,0,0,0,0,0,0,0,0
};

int32_t pawn_pst_eg[64] = {
    0,0,0,0,0,0,0,0,50,37,26,12,6,4,7,14,55,40,20,-7,-8,14,16,17,29,21,7,-16,-9,-5,7,5,12,10,-8,-9,-9,-5,-5,-5,4,0,-4,-7,2,1,-8,-11,11,3,6,0,12,9,-8,-16,0,0,0,0,0,0,0,0
};

int32_t knight_pst_mg[64] = {
    -12,-2,-3,-2,0,-3,-1,-4,-7,-6,4,3,3,5,-3,-2,-5,3,7,12,15,11,8,1,-1,6,12,9,11,34,15,8,-10,0,11,7,19,12,21,-8,-22,-3,6,18,25,13,8,-19,-16,-12,-6,8,4,5,-13,-10,-4,-15,-15,-8,-5,0,-11,-6
};

int32_t knight_pst_eg[64] = {
    -8,-2,0,-3,-1,-2,-2,-4,-8,-3,0,10,6,-3,-5,-6,-5,4,10,7,4,9,1,-6,-4,3,11,12,17,11,8,-5,-8,6,18,17,23,21,9,-7,-17,-3,3,18,16,1,-3,-19,-8,-8,-9,0,2,-10,-9,-9,-6,-25,-11,-7,-9,-13,-15,-4
};

int32_t bishop_pst_mg[64] = {
    -5,-1,-3,-3,-2,-4,0,-1,-15,-7,-4,-3,-2,2,-6,-6,-10,1,12,9,12,11,13,5,-17,8,3,34,19,12,9,-1,-10,-1,0,15,20,-7,-3,1,-12,2,0,2,0,3,2,5,-2,5,4,-10,-5,-2,20,-4,-1,2,-18,-20,-23,-13,-5,-4
};

int32_t bishop_pst_eg[64] = {
    -1,2,1,4,2,1,1,0,-6,5,2,3,4,3,0,-4,-2,8,9,9,10,17,10,5,-3,9,7,18,21,9,10,2,-6,3,13,15,13,9,-1,-3,-3,1,7,8,8,-1,-4,-1,-5,-12,-8,-6,-10,-13,-10,-13,-8,-3,-15,-15,-9,-15,-7,-4
};

int32_t rook_pst_mg[64] = {
    9,10,4,5,6,8,9,13,1,-2,9,13,13,11,6,12,-5,11,10,17,19,21,18,12,-14,-2,3,11,6,16,11,8,-25,-15,-19,-15,-17,-6,3,-5,-31,-22,-25,-21,-21,-13,3,-13,-34,-23,-24,-24,-23,-5,-8,-20,-17,-17,-10,-2,-4,1,1,-14
};

int32_t rook_pst_eg[64] = {
    29,29,28,25,26,26,24,29,14,16,17,20,19,13,13,14,19,18,23,17,15,24,14,16,12,14,17,14,11,16,11,10,-3,4,5,1,0,4,5,-3,-19,-11,-14,-16,-15,-10,-6,-11,-24,-20,-20,-21,-21,-19,-14,-13,-14,-10,-9,-16,-14,-4,-10,-24
};

int32_t queen_pst_mg[64] = {
    -2,3,7,7,13,9,8,13,-20,-43,-6,0,11,19,6,18,-16,-10,-7,5,19,37,43,45,-17,-9,-8,-1,15,23,34,34,-19,-8,-10,-5,-2,13,12,18,-17,-7,-2,-10,-8,0,7,3,-18,-12,-2,-4,-3,-2,-5,-4,-13,-17,-9,2,-8,-24,-8,-6
};

int32_t queen_pst_eg[64] = {
    1,6,11,13,16,12,8,10,-8,-2,1,7,17,16,6,10,-6,-3,2,11,20,29,23,24,-10,2,1,14,26,24,24,21,-7,0,1,17,15,16,10,11,-8,-6,0,-4,-4,2,-1,-1,-10,-11,-28,-18,-22,-23,-14,-4,-10,-12,-17,-23,-18,-17,-6,-4
};

int32_t king_pst_mg[64] = {
    0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,-1,0,3,4,2,2,5,5,-1,0,4,5,4,4,7,6,-2,-2,3,7,3,5,8,7,-7,-3,0,3,-1,1,3,9,-10,2,0,-2,-29,-32,-20,18,30,-12,27,2,-38,-25,-53,26,29
};

int32_t king_pst_eg[64] = {
    -3,-2,-1,-1,-1,-1,-1,-2,-2,5,4,1,1,4,8,-4,0,16,18,8,7,24,22,-3,-2,14,25,23,24,31,22,-6,-11,5,22,29,28,22,10,-19,-11,-2,7,14,13,9,-5,-23,-9,-7,-3,-7,-2,3,-14,-36,-14,-21,-16,-31,-44,-14,-30,-73
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