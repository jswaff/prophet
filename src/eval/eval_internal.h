#ifndef _EVAL_INTERNAL_H_
#define _EVAL_INTERNAL_H_

#include <prophet/bitmap.h>
#include <prophet/eval.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


/* king safety terms */
/*
KING_SAFETY_PAWN_ONE_AWAY=-5
KING_SAFETY_PAWN_FAR_AWAY=-10
KING_SAFETY_PAWN_TWO_AWAY=-9
KING_SAFETY_MIDDLE_OPEN_FILE=-22
*/
static const int32_t king_safety_pawn_one_away       =  -5;
static const int32_t king_safety_pawn_two_away       =  -9;
static const int32_t king_safety_pawn_far_away       = -10;
static const int32_t king_safety_middle_open_file    = -22;


/* pawn terms */
/*
DOUBLED_PAWN=-8
ISOLATED_PAWN=-11
PASSED_PAWN=31
*/
static const int32_t passed_pawn                     =  31;
static const int32_t isolated_pawn                   = -11;
static const int32_t doubled_pawn                    =  -8;


/* knight terms */
/*
KNIGHT_TROPISM=-12
*/
static const int32_t knight_tropism                  = -12;

/* bishop terms */


/* rook terms */
/*
CONNECTED_MAJORS_ON_7TH=76
ROOK_HALF_OPEN_FILE=20
MAJOR_ON_7TH=29
ROOK_OPEN_FILE=35
*/
static const int32_t rook_open_file                  =  35;
static const int32_t rook_half_open_file             =  20;


static const int32_t major_on_7th                    =  29;
static const int32_t connected_majors_on_7th         =  76;


static const int bishop_pst[64] = {
   0,0,0,0,0,0,0,0,-3,7,7,7,8,7,7,-1,-2,9,19,19,19,19,11,5,-1,15,18,30,29,20,17,3,-2,9,18,25,26,14,7,2,-3,8,12,17,13,9,7,4,-1,3,8,-4,-2,3,14,-1,-3,-2,-21,-11,-10,-19,-2,-2
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
   -30,-30,-30,-30,-30,-30,-30,-30,-30,-28,-28,-30,-30,-28,-28,-30,-28,-24,-24,-26,-27,-22,-23,-30,-29,-24,-20,-21,-20,-16,-19,-28,-30,-26,-19,-15,-14,-9,-17,-30,-22,-20,-16,-16,-16,-8,-14,-26,-12,-13,-13,-24,-26,-15,-10,-14,-6,2,-4,-36,-43,-38,-14,-27
};


static const int king_endgame_pst[64] = {
   -17, 70, 43, 74, 66, 57,  8, 21,
    40, 75, 76, 41, 63, 73, 72, 17,
    50, 83, 51, 53, 56, 61, 75, 50,
    19, 57, 52, 25, 39, 52, 60, 40,
    -8, 41, 29, 31, 30, 34, 34, 17,
    -4, -9, 20,  7, 20, 21, 14,  6,
   -39,  5,-15, 13,  9,  9,-11,-25,
   -12,-29,-15, -4,-33, -1,-31,-55  
};

static const int knight_pst[64] = {
   -32,-30,-30,-30,-30,-30,-30,-30,-32,-32,-30,-27,-28,-31,-32,-32,-32,-26,-20,-18,-23,-25,-29,-32,-30,-12,-13,-7,-8,-12,-18,-29,-29,-28,-14,-14,-12,-16,-24,-29,-32,-24,-19,-14,-12,-15,-20,-30,-15,-16,-21,-16,-17,-17,-18,-18,-2,-23,11,-29,-12,-28,-6,-2
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
   0,0,0,0,0,0,0,0,58,49,45,43,38,35,35,38,44,40,36,35,32,33,28,30,7,7,5,6,15,18,12,5,-10,-7,-10,-6,-1,-2,-2,-10,-13,-15,-12,-12,-2,-6,-2,-12,-12,-18,-16,-15,-14,2,-1,-20,0,0,0,0,0,0,0,0
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
   -1,1,1,1,3,1,1,1,-13,-20,-3,0,3,4,1,1,-6,-2,1,5,7,11,9,12,-5,-1,1,5,11,12,11,14,-5,0,1,7,7,11,8,6,-5,-1,2,1,0,3,4,-1,-5,-5,-3,-6,-5,-5,-4,-2,-3,-7,-11,-5,-10,-9,-3,-3
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
   11,9,6,6,4,5,4,6,3,4,6,6,6,3,2,3,4,9,10,10,8,9,5,0,0,4,7,8,7,7,2,-1,-9,-1,-2,-2,-3,0,0,-5,-14,-6,-8,-9,-9,-6,0,-7,-16,-9,-12,-15,-14,-6,-5,-10,-11,-9,-7,-6,-5,2,-3,-22
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
