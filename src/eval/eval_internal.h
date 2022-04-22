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
KING_SAFETY_PAWN_ONE_AWAY=-9
KING_SAFETY_MIDDLE_OPEN_FILE=-14
KING_SAFETY_PAWN_FAR_AWAY=-8
KING_SAFETY_PAWN_TWO_AWAY=-6
*/
static const int32_t king_safety_pawn_one_away       =  -9;
static const int32_t king_safety_pawn_two_away       =  -6;
static const int32_t king_safety_pawn_far_away       =  -8;
static const int32_t king_safety_middle_open_file    = -14;


/* pawn terms */
/*
DOUBLED_PAWN=-9
PASSED_PAWN=34
ISOLATED_PAWN=-13
*/
static const int32_t passed_pawn                     =  34;
static const int32_t isolated_pawn                   = -13;
static const int32_t doubled_pawn                    =  -9;


/* knight terms */
/*
KNIGHT_TROPISM=-5
*/
static const int32_t knight_tropism                  =  -5;

/* bishop terms */


/* rook terms */
/*
MAJOR_ON_7TH=31
CONNECTED_MAJORS_ON_7TH=77
ROOK_OPEN_FILE=42
ROOK_HALF_OPEN_FILE=31
*/
static const int32_t rook_open_file                  =  42;
static const int32_t rook_half_open_file             =  31;


static const int32_t major_on_7th                    =  31;
static const int32_t connected_majors_on_7th         =  77;


static const int bishop_pst[64] = {
   -4,-2,-5,-2,-2,-1,0,-2,-4,7,8,1,10,6,7,-7,-2,9,18,19,17,21,12,0,0,12,24,32,31,23,11,0,-2,13,24,34,30,20,6,-1,-3,13,23,28,24,23,12,3,-1,6,8,3,11,9,28,-4,-11,-2,-22,-7,-8,-20,-5,-9
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
   -31,-31,-30,-31,-30,-28,-28,-30,-29,-24,-23,-25,-25,-21,-22,-28,-26,-20,-16,-22,-23,-9,-12,-24,-28,-18,-11,-13,-14,-5,-10,-31,-35,-26,-11,-8,-12,-5,-18,-40,-27,-21,-12,-16,-12,-6,-12,-35,-24,-16,-12,-30,-28,-13,-15,-32,-21,-17,-18,-51,-39,-44,-20,-43
};


static const int king_endgame_pst[64] = {
   0,0,0,0,0,0,1,1,1,13,11,12,11,13,13,1,3,14,26,22,21,27,19,2,0,14,25,29,30,28,20,0,-3,8,25,29,30,29,13,-4,-3,8,20,23,26,25,17,-6,-5,6,11,9,15,12,10,-12,-7,-9,-9,-5,-16,-8,-25,-25
};

static const int knight_pst[64] = {
   -41,-34,-34,-36,-32,-38,-35,-38,-40,-35,-26,-27,-32,-34,-37,-37,-37,-26,-9,-8,-19,-19,-26,-35,-33,-11,-4,11,13,-5,-9,-30,-32,-26,-5,1,7,-5,-24,-34,-39,-21,-3,-5,-10,10,-10,-37,-19,-20,-26,-11,-11,-14,-18,-18,-6,-35,-4,-32,-19,-30,-37,-5
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
   0,0,0,0,0,0,0,0,107,91,75,68,56,58,61,74,73,72,55,46,44,51,59,56,7,5,-2,1,1,1,3,-2,-18,-18,-22,-12,-12,-22,-20,-29,-24,-23,-26,-27,-20,-19,-15,-26,-26,-23,-34,-45,-33,-7,-10,-32,0,0,0,0,0,0,0,0
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
   -7,4,7,5,9,5,3,6,-19,-22,-1,3,2,8,3,4,-9,-3,4,13,18,15,11,15,-11,-2,5,8,20,17,10,12,-9,-2,6,10,14,13,13,5,-3,-2,6,9,8,12,6,1,-12,-7,13,5,13,4,-2,-1,-4,-11,-8,12,-6,-11,-3,-6
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
   20,13,12,13,12,7,5,7,9,9,9,5,0,7,4,3,7,11,10,11,4,6,9,0,3,3,8,5,4,8,-2,1,-7,-2,1,-2,-3,-2,0,-7,-13,-5,-7,-7,-9,-4,-2,-14,-19,-2,-6,-5,-8,-2,-4,-24,-6,-1,6,8,8,17,-21,-20
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
