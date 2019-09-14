#ifndef _EVAL_INTERNAL_H_
#define _EVAL_INTERNAL_H_

#include <prophet/eval.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


/* king safety terms */
static const int32_t king_safety_pawn_one_away       = -10;
static const int32_t king_safety_pawn_two_away       = -20;
static const int32_t king_safety_pawn_far_away       = -30;
static const int32_t king_safety_middle_open_file    = -50;

/* pawn terms */
static const int32_t passed_pawn                     =  20;
static const int32_t isolated_pawn                   = -20;
static const int32_t doubled_pawn                    = -10;

/* pawn piece square table */
static const int pawn_pst[64] = {
     0,  0,  0,  0,  0,  0,  0,  0,
    30, 30, 30, 30, 30, 30, 30, 30,
    14, 14, 14, 18, 18, 14, 14, 14,
     7,  7,  7, 10, 10,  7,  7,  7,
     5,  5,  5,  7,  7,  5,  5,  5,
     3,  3,  3,  5,  5,  3,  3,  3,
     0,  0,  0, -3, -3,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0 };


/**
 * \brief Evaluate a single pawn.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the pawn is on
 *
 * \return a score for the pawn.
 */
int32_t eval_pawn(const position_t* pos, square_t sq);


/**
 * \brief Determine if a pawn is doubled.
 *
 * \param pos           a pointer to a chess position
 * \param pawn_sq       the pawn square
 *
 * \return true if the pawn is doubled, otherwise false.
 */
bool pawn_doubled(const position_t* pos, square_t pawn_sq);


/**
 * \brief Determine if a pawn is isolated.
 *
 * An isolated pawn is a pawn that has no friendly pawn on an adjacent file.
 *
 * \param pos           a pointer to a chess position
 * \param pawn_sq       the pawn square
 *
 * \return true if the pawn is isolated, otherwise false.
 */
bool pawn_isolated(const position_t* pos, square_t pawn_sq);


/**
 * \brief Determine if a pawn is passed.
 *
 * An passed pawn is a pawn that no enemy pawn can stop from queening.
 *
 * \param pos           a pointer to a chess position
 * \param pawn_sq       the pawn square
 *
 * \return true if the pawn is passed, otherwise false.
 */
bool pawn_passed(const position_t* pos, square_t pawn_sq);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _EVAL_INTERNAL_H_ */
