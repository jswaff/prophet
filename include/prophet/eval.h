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

extern int32_t passed_pawn;
extern int32_t isolated_pawn;
extern int32_t doubled_pawn;

extern int32_t knight_tropism;

extern int32_t bishop_mobility;
extern int32_t bishop_endgame_mobility;

extern int32_t rook_open_file;
extern int32_t rook_half_open_file;

extern int32_t queen_mobility;
extern int32_t queen_endgame_mobility;

extern int32_t major_on_7th;
extern int32_t connected_majors_on_7th;

/* psts */
extern int pawn_pst[64];
extern int pawn_endgame_pst[64];

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
