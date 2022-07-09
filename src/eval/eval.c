#include <prophet/hash.h>
#include "eval_internal.h"

extern hash_table_t phtbl;


/* material values */
int32_t pawn_val                        =  100;
int32_t knight_val                      =  406;
int32_t bishop_val                      =  338;
int32_t bishop_pair                     =   40;
int32_t rook_val                        =  563;
int32_t queen_val                       = 1044;
int32_t knight_kaufman_adj              =    4;
int32_t rook_kaufman_adj                =   -9;

/* king safety */
int32_t king_safety_pawn_one_away       =   -6;
int32_t king_safety_wing_pawn_one_away  =    3;
int32_t king_safety_pawn_two_away       =  -16;
int32_t king_safety_wing_pawn_two_away  =   -4;
int32_t king_safety_pawn_far_away       =  -27;
int32_t king_safety_wing_pawn_far_away  =  -26;
int32_t king_safety_middle_open_file    =  -46;

/* pawn terms */
int32_t passed_pawn_mg[8]               =   { 0,50,40,30,20,10,0,0 };
int32_t passed_pawn_eg[8]               =   { 0,100,70,60,40,20,0,0 };
int32_t isolated_pawn_mg                =  -13;
int32_t isolated_pawn_eg                =  -14;
int32_t doubled_pawn_mg                 =   -5;
int32_t doubled_pawn_eg                 =   -8;

/* knight terms */
int32_t knight_tropism_mg               =   -8;
int32_t knight_tropism_eg               =   -9;

/* bishop terms */
int32_t bishop_mobility_mg              =    4;
int32_t bishop_mobility_eg              =    4;

/* rook terms */
int32_t rook_open_file_mg               =   36;
int32_t rook_open_file_eg               =   27;
int32_t rook_half_open_file_mg          =   20;
int32_t rook_half_open_file_eg          =   14;

/* queen terms */
int32_t queen_mobility_mg               =    0;
int32_t queen_mobility_eg               =    8;

int32_t major_on_7th_mg                 =    6;
int32_t major_on_7th_eg                 =   29;
int32_t connected_majors_on_7th_mg      =   62;
int32_t connected_majors_on_7th_eg      =   62;

/* psts */
int32_t pawn_pst_mg[64] = {
   0,0,0,0,0,0,0,0,108,87,76,70,57,49,44,52,13,43,49,43,42,55,41,22,-22,-19,-14,5,15,20,-2,-15,-32,-32,-21,-13,-7,-9,-14,-33,-32,-39,-27,-23,-10,-21,-9,-30,-38,-43,-41,-32,-33,-16,-6,-33,0,0,0,0,0,0,0,0
};

int32_t pawn_pst_eg[64] = {
   0,0,0,0,0,0,0,0,124,103,89,77,62,52,57,68,84,73,62,47,41,46,49,51,26,16,8,-12,-12,-5,3,2,2,-2,-18,-19,-17,-16,-15,-16,-9,-11,-16,-13,-8,-10,-20,-23,2,-6,-3,-15,-1,2,-17,-25,0,0,0,0,0,0,0,0
};

int32_t knight_pst_mg[64] = {
   -43,-30,-31,-30,-30,-31,-30,-32,-39,-36,-22,-19,-23,-28,-36,-36,-35,-14,2,6,-1,-6,-17,-32,-26,-4,6,12,-1,16,-5,-17,-29,-21,-7,-8,-2,-8,-6,-27,-45,-26,-18,-7,0,-11,-16,-39,-33,-33,-31,-16,-21,-21,-33,-30,-9,-43,-20,-36,-29,-27,-40,-9
};

int32_t knight_pst_eg[64] = {
   -40,-30,-30,-30,-30,-30,-30,-32,-39,-35,-25,-15,-21,-31,-37,-37,-35,-16,2,1,-6,-6,-22,-34,-28,-7,3,12,8,8,-7,-24,-30,-20,0,2,1,-3,-12,-30,-49,-27,-22,-8,-9,-23,-23,-47,-32,-32,-35,-25,-25,-29,-34,-34,-12,-51,-17,-36,-32,-35,-39,-9
};

int32_t bishop_pst_mg[64] = {
   -1,0,0,1,0,-1,0,0,-13,8,7,7,11,11,6,-1,-3,13,26,25,26,29,23,15,-8,16,18,41,31,27,17,5,-4,8,10,21,26,1,7,4,-7,8,5,7,4,7,7,9,-1,9,8,-6,-2,0,25,1,-6,-1,-18,-22,-24,-12,-5,-4
};

int32_t bishop_pst_eg[64] = {
   0,1,1,4,2,1,0,0,-8,13,9,9,12,11,7,-1,-1,15,22,23,24,30,19,13,-2,20,19,30,30,24,23,6,-4,11,20,22,22,15,7,3,-7,7,13,16,16,9,4,7,-3,-4,1,0,0,-5,1,-7,-8,-2,-24,-18,-17,-15,-6,-5
};

int32_t rook_pst_mg[64] = {
   25,24,18,17,18,18,17,26,4,6,14,19,18,15,10,14,5,16,20,21,22,28,19,10,-5,3,10,15,10,17,9,7,-21,-10,-12,-11,-14,-5,0,-7,-34,-22,-25,-25,-24,-19,-2,-18,-40,-29,-29,-30,-29,-14,-14,-25,-21,-21,-17,-13,-14,-9,-6,-21
};

int32_t rook_pst_eg[64] = {
   29,28,24,22,23,24,23,28,15,19,20,24,22,18,14,16,15,17,21,19,18,26,15,14,6,10,13,14,11,15,9,7,-12,-3,-3,-5,-7,-1,1,-6,-28,-16,-20,-20,-20,-15,-4,-15,-34,-25,-25,-27,-27,-18,-15,-20,-17,-15,-10,-16,-14,-4,-11,-26
};

int32_t queen_pst_mg[64] = {
   -1,7,8,11,18,10,9,12,-30,-47,-6,0,12,22,6,14,-17,-8,-1,10,24,43,43,43,-17,-5,-2,7,22,31,35,34,-15,-4,-3,1,3,17,15,18,-17,-7,-2,-8,-6,0,8,0,-20,-12,-4,-7,-7,-7,-8,-4,-13,-23,-19,-8,-18,-28,-11,-6
};

int32_t queen_pst_eg[64] = {
   -1,8,11,13,18,12,8,11,-25,-33,-5,2,16,22,6,10,-15,-6,1,12,25,40,37,40,-15,-3,-1,12,27,32,34,34,-14,-3,-1,11,10,19,16,17,-14,-6,-1,-6,-6,2,6,-1,-17,-12,-16,-15,-15,-16,-14,-5,-11,-20,-25,-20,-25,-27,-10,-6
};

int32_t king_pst_mg[64] = {
   -30,-30,-30,-30,-30,-30,-30,-30,-30,-26,-26,-30,-30,-26,-26,-30,-27,-18,-17,-24,-25,-15,-16,-29,-27,-17,-10,-15,-14,-6,-10,-28,-30,-21,-10,-9,-8,-2,-9,-32,-24,-20,-14,-17,-17,-9,-14,-34,-14,-17,-17,-45,-50,-35,-7,3,-22,-3,-29,-66,-58,-74,-6,-4
};

int32_t king_pst_eg[64] = {
   -31,-30,-30,-30,-30,-30,-30,-30,-30,-19,-25,-29,-29,-21,-17,-30,-26,-7,-5,-15,-16,2,2,-28,-27,-6,5,0,0,10,2,-28,-31,-17,1,4,4,0,-10,-36,-33,-21,-14,-10,-11,-13,-24,-40,-24,-26,-24,-30,-26,-20,-31,-51,-23,-29,-33,-58,-67,-39,-47,-81
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