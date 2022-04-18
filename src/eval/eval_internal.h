#ifndef _EVAL_INTERNAL_H_
#define _EVAL_INTERNAL_H_

#include <prophet/bitmap.h>
#include <prophet/eval.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


/* king safety terms */
static const int32_t king_safety_pawn_one_away       =   8;
static const int32_t king_safety_pawn_two_away       = -11;
static const int32_t king_safety_pawn_far_away       = -24;
static const int32_t king_safety_middle_open_file    = -62;


/* pawn terms */
static const int32_t passed_pawn                     =  36;
static const int32_t isolated_pawn                   = -13;
static const int32_t doubled_pawn                    =  -6;


/* knight terms */
static const int32_t knight_tropism                  =  -3;

/* bishop terms */


/* rook terms */
static const int32_t rook_open_file                  =  42;
static const int32_t rook_half_open_file             =  30;


static const int32_t major_on_7th                    =  28;
static const int32_t connected_majors_on_7th         =  76;


static const int bishop_pst[64] = {
   -3,  0, -4, -3, -2, -1,  0, -2,
   -5,  6,  8,  2, 11, 10,  9, -7,
   -4, 10, 20, 19, 15, 19, 12,  2,
   -1, 10, 22, 33, 29, 25,  9, -1,
    1, 12, 25, 34, 31, 19,  8, -1,
   -4, 14, 24, 26, 23, 25, 15, -1,
    1,  7, 10, -1, 10,  9, 26, -7,
  -13, -1,-25, -7, -6,-18, -3, -8
};

static const int bishop_endgame_pst[64] = {
  -22, -15,  5,-35, -5,  5,  8,-13,
  -23, -19, -7, -4,  3, -5, -9,-32,
   -7,   1, -2,  0,  7, 25, 26,  4,
  -13,  -7,  9, 15,  1,  2,-14, -3,
  -22,  -1,  3,  9,  6,-10, -8,-19,
  -10, -12, -3, -9,  0,  2,-10,-25,
   -4, -22,-24,-22,-11, -5,-13, -9,
  -57, -15,-48,-22,-25,-38,-21,-56  
};


static const int king_pst[64] = {
   -30,-29,-30,-30,-30,-30,-29,-30,
   -30,-29,-29,-29,-29,-29,-29,-30,
   -29,-28,-28,-29,-29,-28,-26,-29,
   -30,-29,-27,-29,-29,-28,-27,-31,
   -31,-30,-28,-30,-31,-29,-31,-33,
   -21,-20,-20,-22,-22,-22,-16,-24,
   -12, -8, -9,-30,-30,-10,  8,  4,
    -5, 15, 19,-39,  1,-44, 42,  1
};


static const int king_endgame_pst[64] = {
   -2,  0,  0, -1,  0,  1,  2,  1,  
    0, 14, 14, 14, 13, 17, 15,  2,
    5, 16, 25, 21, 22, 30, 28,  4,
    1, 18, 32, 33, 28, 33, 26,  0,
   -4,  6, 29, 33, 32, 32, 13, -6,
   -4,  6, 18, 24, 28, 23, 17, -9,
   -6,  5, 10, 10, 18, 16,  6,-18,
  -12,-15,-12, -8,-24, -8,-35,-37
};

static const int knight_pst[64] = {
   -41,-34,-33,-35,-30,-38,-35,-37,
   -38,-34,-26,-29,-31,-32,-36,-38,
   -36,-24,-11, -8,-18,-18,-26,-37,
   -31,-15, -5, 13, 15, -7,-11,-32,
   -36,-27, -5,  0,  4, -5,-25,-32,
   -40,-24,  0, -7, -5, 10,-11,-33,
   -20,-20,-25,-12,-10,-11,-19,-17,
    -8,-38, -3,-34,-17,-32,-36, -5
};

static const int knight_endgame_pst[64] = {
   -82,-16,  8, -3, -7, -7,-17,-75,
   -25, -6, 16, 15, -3,-15,  7,-70,
    -9,  3, 25, 18, 30, 33, 21,-15,
    -7,  9, 22, 42, 12, 27,-14, -9,
   -19,  8,  6, -4,  0, 13, 16,-21,
   -39,-33,-14,  7, 14,-10, -3,-43,
   -56,-41,-17,-10,-13,-32,-31, -3,
   -72,-27,-56,-45,-45,-41,-28,-98  
 };


static const int pawn_pst[64] = {
   0,  0,  0,  0,  0,  0,  0,  0,
 108, 93, 73, 69, 57, 58, 64, 76,
  75, 72, 55, 45, 40, 51, 57, 56,
   8,  7, -1,  0,  0,  2,  1, -2,
 -20,-18,-17, -9,-11,-22,-21,-26,
 -24,-23,-25,-26,-20,-24,-20,-30,
 -25,-23,-33,-47,-34, -7,-11,-34,
   0,  0,  0,  0,  0,  0,  0,  0
};

static const int pawn_endgame_pst[64] = {
     0,  0,  0,  0,  0,  0,  0,  0,
   127,108,112,120, 76, 89,106, 83,
    69, 47, 63, 48, 45, 47, 58, 37,
    17,  2,  2,  5,  5,  2,  11, 8,
     0, -6, -6, -8, -6,  0, -9,-10,
   -11,-11,-14,-14,  1, -2, -6, -8,
    -5,-17,-18,-25,-11,  6, -5,-17,
     0,  0,  0,  0,  0,  0,  0,  0
};


static const int queen_pst[64] = {
   -7,2,7,6,6,5,4,6,-19,-22,-2,2,4,9,4,6,-8,-4,5,13,14,16,10,12,-7,-5,4,10,17,18,11,10,-10,-3,7,11,14,12,14,6,-7,1,8,6,9,14,11,2,-12,-4,14,6,13,2,-5,-2,-2,-12,-8,11,-9,-11,-2,-6
};

static const int queen_endgame_pst[64] = {
   19,  49, 63, 55, 53, 99, 87, 65,
   -6,  12, 32, 58, 75, 79, 53, 58,
   -4,  20, 40, 56, 83, 99, 95, 53,
  -12,   7, 26, 48, 41, 60, 43, 31,
  -11,  15, 10, 25, 29, 45, 27, 36,
   -9,   3,  3,  8, 21, 15,  4,  7,
  -17,  -3,  3,  1,  6, 13, 15, 19,
  -18, -10, -9, -6, -9,-12,-26, 28  
};

static const int rook_pst[64] = {
   18,13,11,10,11,7,6,12,9,9,11,5,2,9,6,3,10,13,9,7,5,3,8,-1,3,2,7,3,3,7,0,2,-6,-2,0,-1,-3,-2,-2,-4,-15,-3,-8,-7,-9,-1,0,-13,-20,-4,-5,-4,-9,2,-5,-25,-3,-2,4,6,4,11,-17,-15
};

static const int rook_endgame_pst[64] = {
     2, 10, 10, 19, 14,  6, -2,  2,
     4,  7, 10, 11,  6, 13, 12, 10,
    -4,  2,  4, 11, 10, 11,  7,  7,
    -6, -3, -9, -2,-21,  0,  4,-15,
   -31, -3,-21,-21,-13,-13,-20,-16,
   -38,-29,-37,-26,-29,-27,-22,-33,
   -53,-35,-35,-37,-33,-23,-31,-39,
   -35,-31,-28,-23,-22,-25,-41,-43
};

enum material_type_t { KK, KKN, KKB, KKNN, KPKN, KPKB, KNK, KNKP, KNKN, KNKB, 
                       KNNK, KBK, KBKP, KBKN, KBKB, OTHER };
typedef enum material_type_t material_type_t;


/* define a generic signature for piece type eval functions */
typedef int32_t (*eval_func_t)(const position_t* pos, square_t sq, bool endgame);


/**
 * \brief Evaluate a single bishop.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the bishop is on
 * \param endgame       if the eval should be done in the endgame phase 
 *
 * \return a score for the bishop.
 */
int32_t eval_bishop(const position_t* pos, square_t sq, bool endgame);


/**
 * \brief Evaluate the position for bishop pairs.
 *
 * \param pos           a pointer to a chess position
 *
 * \return a score for the bishop.
 */
int32_t eval_bishop_pair(const position_t* pos);


/**
 * \brief Evaluate a single king.
 * 
 * TODO: separate endgame evaluation to another method.  Do not scale king
 * safety in this function.  Instead, try to smooth the two scores based
 * on material on the board.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the king is on
 * \param endgame       if the eval should be done in the endgame phase 
 *
 * \return a score for the king.
 */
int32_t eval_king(const position_t* pos, square_t sq, bool endgame);


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
 * \param endgame       if the eval should be done in the endgame phase 
 *
 * \return a score for the knight.
 */
int32_t eval_knight(const position_t* pos, square_t sq, bool endgame);


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
 * \brief Evaluate the position's "material type".
 *
 * \param pos           a pointer to a chess position
 * \param draw_flag     a pointer to an integer to set the IMMEDIATE_DRAW flag
 *
 * \return the material type
 */
material_type_t eval_material_type(const position_t* pos, int* draw_flag);

/**
 * \brief Evaluate a single pawn.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the pawn is on
 * \param endgame       if the eval should be done in the endgame phase 
 *
 * \return a score for the pawn.
 */
int32_t eval_pawn(const position_t* pos, square_t sq, bool endgame);


/**
 * \brief Evaluate a single queen.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the queen is on
 * \param endgame       if the eval should be done in the endgame phase 
 *
 * \return a score for the queen.
 */
int32_t eval_queen(const position_t* pos, square_t sq, bool endgame);


/**
 * \brief Evaluate a single rook.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the rook is on
 * \param endgame       if the eval should be done in the endgame phase 
 *
 * \return a score for the rook.
 */
int32_t eval_rook(const position_t* pos, square_t sq, bool endgame);


/**
 * \brief Calculate a tapered score.
 *
 * Given a "middle game score", an "end game score", calculate a blended
 * score in the range [mg, eg] depending on how much material is on the
 * board.  Positions with the majority of major/minor pieces will be 
 * weighted heavily towards the middle game score.  Likewise, positions with
 * few or no major/minor pieces will be weighted towards the end game
 * score.

 * \param pos           a pointer to a chess position
 * \param mg_score      the middle game score
 * \param eg_score      the end game score
 *
 * \return the tapered score
 */
int32_t eval_taper(const position_t* pos, int32_t mg_score, int32_t eg_score);


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


/**
 * \brief Accumulate the score for a set of pieces.
 *
 * \param pos           a pointer to a chess position
 * \param piece_map     a set of pieces to evaluate
 * \param endgame       if the eval should be done in the endgame phase 
 * \param eval_func     the evaluation function to use for each piece
 *
 * \return the cumulative score.
 */
int32_t eval_accumulator(const position_t* pos, uint64_t piece_map, 
  bool endgame, eval_func_t eval_func);



// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _EVAL_INTERNAL_H_ */
