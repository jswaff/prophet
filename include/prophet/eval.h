#ifndef _EVAL_H_
#define _EVAL_H_

#include <prophet/position/position.h>


// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus


/*
BISHOP_PAIR=32
PAWN_VAL=100
ROOK_VAL=517
KNIGHT_VAL=383
QUEEN_VAL=985
BISHOP_VAL=329
*/


static const int32_t pawn_val = 100;
static const int32_t knight_val = 383;
static const int32_t bishop_val = 329;
static const int32_t bishop_pair = 32;
static const int32_t rook_val = 517;
static const int32_t queen_val = 985;

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
