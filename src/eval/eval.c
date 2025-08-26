#include "prophet/eval.h"

#include "prophet/position.h"
#include "prophet/square.h"

#include "eval_internal.h"
#include "bitmap/bitmap.h"
#include "hash/hash_internal.h"
#include "position/square_internal.h"

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

extern hash_table_t phtbl;

/*
#Error: 0.11667905358000843
#Mon Oct 10 16:40:04 EDT 2022
*/

/* material values */
int32_t pawn_val                        =  100;
int32_t knight_val                      =  373;
int32_t bishop_val                      =  361;
int32_t bishop_pair                     =   40;
int32_t rook_val                        =  558;
int32_t queen_val                       = 1091;
int32_t knight_kaufman_adj              =    4;
int32_t rook_kaufman_adj                =   -5;


/* king safety */
int32_t king_safety_pawn_one_away       =   -6;
int32_t king_safety_wing_pawn_one_away  =   10;
int32_t king_safety_pawn_two_away       =  -19;
int32_t king_safety_wing_pawn_two_away  =    6;
int32_t king_safety_pawn_far_away       =  -28;
int32_t king_safety_wing_pawn_far_away  =  -18;
int32_t king_safety_middle_open_file    =  -54;


/* pawn terms */
int32_t passed_pawn_mg[8]               =   { 0,85,47,18,-14,-20,-9,0 };
int32_t passed_pawn_eg[8]               =   { 0,156,112,63,48,25,19,0 };
int32_t isolated_pawn_mg                =  -11;
int32_t isolated_pawn_eg                =  -14;
int32_t doubled_pawn_mg                 =   -7;
int32_t doubled_pawn_eg                 =   -8;


int32_t knight_tropism_mg               =   -6;
int32_t knight_tropism_eg               =   -6;
int32_t knight_outpost[64]              =    { -20,-4,-3,-4,-2,-4,-2,-8,-14,-9,4,13,9,2,-8,-9,-9,7,21,21,24,25,11,-4,2,10,19,26,24,25,19,9,2,9,11,16,14,14,8,7,-12,5,6,2,3,2,1,-4,-11,-3,-4,-1,-1,-4,-2,-12,-6,-11,-5,-4,-3,-6,-2,-3 };
int32_t knight_supported_outpost[64]    =    { 0,0,0,0,0,0,0,0,0,1,1,2,2,1,1,0,1,10,16,17,13,16,10,2,4,17,18,20,20,18,17,6,10,6,19,18,10,12,6,3,-5,-1,-5,0,-2,-6,-2,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };


/* bishop terms */
int32_t bishop_mobility_mg[14]          =    { -25,-19,-14,-10,-5,2,3,3,5,10,17,20,5,7 };
int32_t bishop_mobility_eg[14]          =    { -20,-36,-29,-17,-1,10,18,24,27,25,21,21,8,11 };
int32_t bishop_trapped                  =   -125;


/* rook terms */
int32_t rook_mobility_mg[15]            =    { -35,-30,-25,-24,-26,-21,-20,-15,-11,-11,-4,-4,6,16,13 };
int32_t rook_mobility_eg[15]            =    { -21,-20,-14,-2,14,19,25,27,31,36,36,37,32,26,24 };
int32_t rook_open_file_mg               =   30;
int32_t rook_open_file_supported_mg     =    0; /* disabled */
int32_t rook_open_file_eg               =   19;
int32_t rook_open_file_supported_eg     =    0; /* disabled */
int32_t rook_half_open_file_mg          =   18;
int32_t rook_half_open_file_supported_mg =   0; /* disabled */
int32_t rook_half_open_file_eg          =   13;
int32_t rook_half_open_file_supported_eg =   0; /* disabled */


/* queen terms */
int32_t queen_mobility_mg[28]           =    { -2,-8,-12,-10,-10,-9,-7,-6,-3,0,2,4,6,6,9,11,13,12,16,14,12,11,5,4,2,1,0,0 };
int32_t queen_mobility_eg[28]           =    { 0,-1,-3,-6,-9,-13,-16,-10,-7,-3,1,6,10,14,19,18,18,18,20,20,17,16,8,7,4,2,1,1 };

int32_t major_on_7th_mg                 =  -17;
int32_t major_on_7th_eg                 =   50;
int32_t connected_majors_on_7th_mg      =   10;
int32_t connected_majors_on_7th_eg      =   15;


/* psts */
int32_t pawn_pst_mg[64] = {
    0,0,0,0,0,0,0,0,26,20,15,13,8,5,-1,0,-14,6,13,5,13,39,13,3,-24,-14,-13,7,17,24,6,-11,-29,-24,-17,-9,-2,-3,-4,-31,-28,-32,-22,-17,-6,-13,-1,-25,-32,-36,-37,-29,-29,-11,3,-22,0,0,0,0,0,0,0,0
};

int32_t pawn_pst_eg[64] = {
    0,0,0,0,0,0,0,0,51,38,25,11,6,3,8,14,56,39,20,-6,-9,14,15,18,29,21,7,-17,-9,-5,7,5,12,10,-7,-9,-9,-5,-5,-6,4,0,-4,-7,2,0,-9,-11,11,3,6,0,13,9,-8,-16,0,0,0,0,0,0,0,0
};

int32_t knight_pst_mg[64] = {
    -12,-1,-3,-2,-1,-3,-1,-4,-7,-6,4,4,3,5,-3,-2,-4,3,7,11,14,11,8,2,0,6,12,10,10,36,14,8,-12,2,10,6,19,12,22,-8,-22,-3,6,18,26,13,8,-20,-15,-12,-7,8,4,4,-13,-10,-4,-15,-13,-7,-4,-1,-13,-6
};

int32_t knight_pst_eg[64] = {
    -8,-3,0,-2,-2,-1,-2,-4,-7,-3,0,9,6,-3,-5,-7,-5,4,10,8,5,9,1,-5,-4,3,12,12,18,11,6,-6,-7,5,18,18,23,20,10,-8,-16,-3,4,19,16,1,-3,-19,-8,-8,-8,-1,2,-10,-10,-10,-6,-26,-10,-7,-9,-13,-15,-4
};

int32_t bishop_pst_mg[64] = {
    -4,-1,-3,-3,-2,-5,0,-1,-15,-9,-5,-3,-2,2,-4,-5,-11,2,11,8,11,11,13,8,-16,8,3,34,19,12,9,-1,-11,-3,1,15,21,-7,-2,1,-13,2,-2,4,0,3,1,7,-2,4,5,-10,-6,-2,21,-2,-1,1,-18,-21,-23,-12,-5,-4
};

int32_t bishop_pst_eg[64] = {
    0,3,1,3,2,-1,0,0,-5,4,1,2,3,3,0,-3,-2,9,8,9,10,17,10,4,-3,9,7,19,22,9,12,3,-5,3,12,15,13,10,-1,-3,-5,1,6,7,8,-2,-4,-1,-6,-11,-8,-6,-11,-12,-10,-12,-8,-3,-14,-13,-10,-16,-6,-4
};

int32_t rook_pst_mg[64] = {
    9,11,4,5,6,9,9,12,1,-2,10,13,13,11,6,12,-5,11,10,18,19,22,18,11,-13,-2,3,13,5,14,10,9,-25,-15,-18,-16,-17,-6,3,-4,-31,-23,-24,-21,-22,-12,4,-12,-33,-25,-24,-24,-22,-6,-7,-20,-17,-17,-11,-2,-5,2,2,-13
};

int32_t rook_pst_eg[64] = {
    29,29,28,25,25,26,25,29,13,17,16,20,18,13,13,13,19,17,23,17,15,24,14,16,11,13,17,13,11,15,10,11,-3,3,6,1,0,4,5,-2,-19,-11,-14,-15,-14,-10,-6,-11,-24,-20,-19,-21,-22,-20,-13,-13,-14,-11,-8,-17,-13,-4,-11,-23
};

int32_t queen_pst_mg[64] = {
    -2,4,6,8,12,9,8,12,-21,-43,-5,0,12,19,7,18,-16,-10,-7,4,19,36,44,46,-16,-9,-7,-2,14,24,33,35,-18,-8,-9,-6,-2,13,13,18,-18,-8,-3,-11,-7,-1,7,1,-18,-11,-2,-5,-2,-3,-5,-3,-13,-17,-10,2,-8,-25,-8,-5
};

int32_t queen_pst_eg[64] = {
    1,7,10,13,16,11,8,10,-8,-2,1,7,17,16,6,10,-6,-4,2,10,20,28,24,24,-9,2,2,14,26,24,24,21,-7,-1,2,17,14,15,10,11,-9,-4,1,-3,-4,3,-1,-1,-9,-12,-28,-18,-23,-23,-15,-4,-10,-12,-17,-24,-18,-17,-6,-3
};

int32_t king_pst_mg[64] = {
    0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,-1,0,3,4,2,2,5,5,-1,0,3,5,4,4,7,6,-2,-2,2,7,4,5,8,7,-7,-3,0,3,-1,1,3,9,-9,0,0,-2,-29,-32,-21,18,30,-11,27,0,-37,-26,-53,27,30
};

int32_t king_pst_eg[64] = {
    -3,-1,-1,-1,-1,-1,-1,-3,-2,5,3,1,1,4,8,-3,1,15,18,9,8,24,23,-3,-2,14,26,22,23,30,22,-7,-10,5,23,29,28,23,11,-19,-11,-1,7,15,13,9,-6,-22,-9,-7,-2,-7,-3,2,-14,-36,-14,-21,-16,-30,-43,-14,-32,-73
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
 * \param fen             a chess position
 * \param material_only   if the evaluation should consider material only
 * \param use_pawn_hash   if the pawn hash table should be used
 *
 * \return the score.
 */
int32_t eval_from_fen(const char *fen, bool material_only) {
    position_t pos;
    set_pos(&pos, fen);
    return eval(&pos, material_only, false);
}


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
 * \param use_pawn_hash   if the pawn hash table should be used
 *
 * \return the score.
 */
int32_t eval(const position_t* pos, bool material_only, bool use_pawn_hash)
{
    /* establish a baseline score using material, from white's perspective. */
    int32_t mat_score =
        eval_nonpawn_material(pos, true) -     /* white non-pawn material */
        eval_nonpawn_material(pos, false) +    /* black non-pawn material */
        eval_pawn_material(pos, true) -        /* white pawn material */
        eval_pawn_material(pos, false);        /* black pawn material */

    if (material_only) {
        return pos->player == WHITE ? mat_score : -mat_score;
    }

    /* evaluate for a draw.  positions that are drawn by rule are immediately
     * returned.  Others that are "drawish" are further evaluated but later
     * tapered down.
     */
    int immediate_draw = 0;
    material_type_t mt = eval_material_type(pos, &immediate_draw);
    if (immediate_draw) {
        return 0;
    }

    int draw_factor = 1;
    if (KPKN==mt || KPKB==mt || KNKP==mt || KBKP==mt) {
        draw_factor = 8;
    }

    int32_t mg_score = mat_score;
    int32_t eg_score = mat_score;

    /* fold in pawn positional features. try the pawn hash first. */
    uint64_t pawn_hash_val = use_pawn_hash ? probe_hash(&phtbl, pos->pawn_key) : 0;
    if (pawn_hash_val != 0) {
        mg_score += get_pawn_hash_entry_mg_score(pawn_hash_val);
        eg_score += get_pawn_hash_entry_eg_score(pawn_hash_val);
        assert(verify_pawn_scores(pos, mg_score - mat_score, eg_score - mat_score));
    } else {
        uint64_t all_pawns = pos->white_pawns | pos->black_pawns;
        while (all_pawns) {
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
    while (all_knights) {
        square_t sq = (square_t)get_lsb(all_knights);
        eval_knight(pos, sq, &mg_score, &eg_score);
        all_knights ^= square_to_bitmap(sq);
    }

    /* fold in bishop positional features */
    uint64_t all_bishops = pos->white_bishops | pos->black_bishops;
    while (all_bishops) {
        square_t sq = (square_t)get_lsb(all_bishops);
        eval_bishop(pos, sq, &mg_score, &eg_score);
        all_bishops ^= square_to_bitmap(sq);
    }

    /* fold in rook positional features */
    uint64_t all_rooks = pos->white_rooks | pos->black_rooks;
    while (all_rooks) {
        square_t sq = (square_t)get_lsb(all_rooks);
        eval_rook(pos, sq, &mg_score, &eg_score);
        all_rooks ^= square_to_bitmap(sq);
    }

    /* fold in queen positional features */
    uint64_t all_queens = pos->white_queens | pos->black_queens;
    while (all_queens) {
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
    while (all_pawns) {
        square_t sq = (square_t)get_lsb(all_pawns);
        eval_pawn(pos, sq, &my_mg_score, &my_eg_score);
        all_pawns ^= square_to_bitmap(sq);
    }

    assert(mg_score==my_mg_score);
    assert(eg_score==my_eg_score);

    return mg_score==my_mg_score && eg_score==my_eg_score;
}
#endif

