#ifndef _EVAL_INTERNAL_H_
#define _EVAL_INTERNAL_H_

#include <prophet/eval.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

/* piece values */
static const int32_t pawn_val = 100;
static const int32_t knight_val = 300;
static const int32_t bishop_val = 320;
static const int32_t rook_val = 500;
static const int32_t queen_val = 900;
static inline int32_t all_nonpawn_pieces_val()
{
  return  queen_val + rook_val*2 + bishop_val*2 + knight_val*2;
}  

/* king safety terms */
static const int32_t king_safety_pawn_one_away       = -10;
static const int32_t king_safety_pawn_two_away       = -20;
static const int32_t king_safety_pawn_far_away       = -30;
static const int32_t king_safety_middle_open_file    = -50;


/* pawn terms */
static const int32_t passed_pawn                     =  20;
static const int32_t isolated_pawn                   = -20;
static const int32_t doubled_pawn                    = -10;


/* knight terms */
static const int32_t knight_tropism                  =  -2;


/* rook terms */
static const int32_t rook_open_file                  =  25;
static const int32_t rook_half_open_file             =  15;


static const int32_t major_on_7th                    =  50;
static const int32_t connected_majors_on_7th         =  80;


/* bishop piece square table */
static const int bishop_pst[64] = {
      0, 0,  0,  0,  0,  0, 0, 0,
      0, 7,  7,  7,  7,  7, 7, 0,
      0, 7, 15, 15, 15, 15, 7, 0,
      0, 7, 15, 20, 20, 15, 7, 0,
      0, 7, 15, 20, 20, 15, 7, 0,
      0, 7, 15, 15, 15, 15, 7, 0,
      0, 7,  7,  7,  7,  7, 7, 0,
      0, 0,  0,  0,  0,  0, 0, 0 };


/* king piece square table for non-endgame.
 * the idea is to encourage safety. */
static const int king_pst[64] = {
   -30,-30,-30,-30,-30,-30,-30,-30,
   -30,-30,-30,-30,-30,-30,-30,-30,
   -30,-30,-30,-30,-30,-30,-30,-30,
   -30,-30,-30,-30,-30,-30,-30,-30,
   -30,-30,-30,-30,-30,-30,-30,-30,
   -20,-20,-20,-20,-20,-20,-20,-20,
   -10,-10,-10,-10,-10,-10,-10,-10,
     0, 10, 20,-25,  0,-25, 20,  0 };


/* king piece square table for endgame.
 * this table encourages centrality. */
static const int king_endgame_pst[64] = {
     0,  0,  0,  0,  0,  0,  0,  0,
     0, 10, 10, 10, 10, 10, 10,  0,
     0, 10, 20, 20, 20, 20, 10,  0,
     0, 10, 20, 25, 25, 20, 10,  0,
     0, 10, 20, 25, 25, 20, 10,  0,
     0, 10, 20, 20, 20, 20, 10,  0,
     0, 10, 10, 10, 10, 10, 10,  0,
     0,  0,  0,  0,  0,  0,  0,  0 };


/* knight piece square table */
static const int knight_pst[64] = {
     -5, -5, -5, -5, -5, -5, -5, -5,
     -5,  0, 10, 10, 10, 10,  0, -5,
     -5,  0, 15, 20, 20, 15,  0, -5,
     -5,  5, 10, 15, 15, 10,  5, -5,
     -5,  0, 10, 15, 15, 10,  5, -5,
     -5,  0,  8,  0,  0,  8,  0, -5,
     -5,  0,  0,  5,  5,  0,  0, -5,
    -10,-10, -5, -5, -5, -5,-10,-10 };


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


/* queen piece square table */
static const int queen_pst[64] = {
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1,  0,  0,  0,  0,  0,  0, -1,
    -1,  0,  1,  1,  1,  1,  0, -1,
    -1,  0,  1,  2,  2,  1,  0, -1,
    -1,  0,  1,  2,  2,  1,  0, -1,
    -1,  0,  1,  1,  1,  1,  0, -1,
    -1,  0,  0,  0,  0,  0,  0, -1,
    -1, -1, -1, -1, -1, -1, -1, -1 };


/* rook piece square table */
static const int rook_pst[64] = {
     0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
     0,  0,  0,  0,  0,  0,  0,  0 };


/**
 * \brief Scale a score by non-pawn material on the board.
 *
 * \param score         the score to scale
 * \param material      the amount of non-pawn material on the board
 *
 * \return the scaled score.
 */
static inline int32_t eval_scale(int32_t score, int32_t material)
{
    return score * material / all_nonpawn_pieces_val();
}

/**
 * \brief Evaluate a single bishop.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the bishop is on
 *
 * \return a score for the bishop.
 */
int32_t eval_bishop(const position_t* pos, square_t sq);


/**
 * \brief Evaluate a single king.
 * 
 * TODO: separate endgame evaluation to another method.  Do not scale king
 * safety in this function.  Instead, try to smooth the two scores based
 * on material on the board.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the king is on
 * \param enemy_np_mat  the enemy's non-pawn material
 *
 * \return a score for the king.
 */
int32_t eval_king(const position_t* pos, square_t sq, int32_t enemy_np_mat);


/**
 * \brief Evaluate king safety for one side of the board.
 *
 * \param pos           a pointer to a chess position
 * \param wtm           white to move?
 *
 * \return a score of the king safety for the player specified.
 */
int32_t eval_king_safety(const position_t* pos, bool wtm);


/**
 * \brief Evaluate a single knight.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the knight is on
 *
 * \return a score for the knight.
 */
int32_t eval_knight(const position_t* pos, square_t sq);


/**
 * \brief Evaluate a connected major on the 7th rank.
 *
 * A major piece is a rook or a queen.  For black, the "7th rank" is the
 * "2nd rank".
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the major piece is on
 *
 * \return a score 
 */
int32_t eval_major_on_7th(const position_t* pos, square_t sq);


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
 * \brief Evaluate a single queen.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the queen is on
 *
 * \return a score for the queen.
 */
int32_t eval_queen(const position_t* pos, square_t sq);


/**
 * \brief Evaluate a single rook.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the rook is on
 *
 * \return a score for the rook.
 */
int32_t eval_rook(const position_t* pos, square_t sq);


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
