#ifndef _EVAL_H_
#define _EVAL_H_

#include <prophet/position/position.h>


// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus


extern int32_t pawn_val;
extern int32_t knight_val;
extern int32_t bishop_val;
extern int32_t bishop_pair;
extern int32_t rook_val;
extern int32_t queen_val;
extern int32_t knight_kaufman_adj;
extern int32_t rook_kaufman_adj;

extern int32_t king_safety_pawn_one_away;
extern int32_t king_safety_wing_pawn_one_away;
extern int32_t king_safety_pawn_two_away;
extern int32_t king_safety_wing_pawn_two_away;
extern int32_t king_safety_pawn_far_away;
extern int32_t king_safety_wing_pawn_far_away;
extern int32_t king_safety_middle_open_file;

extern int32_t passed_pawn_mg[8];
extern int32_t passed_pawn_eg[8];
extern int32_t isolated_pawn_mg;
extern int32_t isolated_pawn_eg;
extern int32_t doubled_pawn_mg;
extern int32_t doubled_pawn_eg;

extern int32_t knight_tropism_mg;
extern int32_t knight_tropism_eg;
extern int32_t knight_outpost[64];
extern int32_t knight_supported_outpost[64];

extern int32_t bishop_mobility_mg[14];
extern int32_t bishop_mobility_eg[14];
extern int32_t bishop_trapped;

extern int32_t rook_mobility_mg[15];
extern int32_t rook_mobility_eg[15];
extern int32_t rook_open_file_mg;
extern int32_t rook_open_file_supported_mg;
extern int32_t rook_open_file_eg;
extern int32_t rook_open_file_supported_eg;
extern int32_t rook_half_open_file_mg;
extern int32_t rook_half_open_file_supported_mg;
extern int32_t rook_half_open_file_eg;
extern int32_t rook_half_open_file_supported_eg;

extern int32_t queen_mobility_mg[28];
extern int32_t queen_mobility_eg[28];

extern int32_t major_on_7th_mg;
extern int32_t major_on_7th_eg;
extern int32_t connected_majors_on_7th_mg;
extern int32_t connected_majors_on_7th_eg;

/* psts */
extern int32_t pawn_pst_mg[64];
extern int32_t pawn_pst_eg[64];
extern int32_t knight_pst_mg[64];
extern int32_t knight_pst_eg[64];
extern int32_t bishop_pst_mg[64];
extern int32_t bishop_pst_eg[64];
extern int32_t rook_pst_mg[64];
extern int32_t rook_pst_eg[64];
extern int32_t queen_pst_mg[64];
extern int32_t queen_pst_eg[64];
extern int32_t king_pst_mg[64];
extern int32_t king_pst_eg[64];


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
int32_t eval(const position_t* pos, bool material_only);


/**
 * \brief Evaluate the position's non-pawn material.
 *
 * \param pos           a pointer to a chess position
 * \param for_white     whether the returned value should be for white
 *
 * \return the score.
 */
int32_t eval_nonpawn_material(const position_t* pos, bool for_white);


/**
 * \brief Evaluate the position's pawn material.
 *
 * \param pos           a pointer to a chess position
 * \param for_white     whether the returned value should be for white
 *
 * \return the score.
 */
int32_t eval_pawn_material(const position_t* pos, bool for_white);



// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _EVAL_H_ */
