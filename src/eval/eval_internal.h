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
KING_SAFETY_PAWN_FAR_AWAY=-22
KING_SAFETY_PAWN_TWO_AWAY=-10
KING_SAFETY_PAWN_ONE_AWAY=-1
KING_SAFETY_MIDDLE_OPEN_FILE=-31
*/
static const int32_t king_safety_pawn_one_away       =  -1;
static const int32_t king_safety_pawn_two_away       = -10;
static const int32_t king_safety_pawn_far_away       = -22;
static const int32_t king_safety_middle_open_file    = -31;


/* pawn terms */
/*
ISOLATED_PAWN=-13
DOUBLED_PAWN=-8
PASSED_PAWN=28
*/
static const int32_t passed_pawn                     =  28;
static const int32_t isolated_pawn                   = -13;
static const int32_t doubled_pawn                    =  -8;


/* knight terms */
/*
KNIGHT_TROPISM=-8
*/
static const int32_t knight_tropism                  =  -8;

/* bishop terms */


/* rook terms */
/*
MAJOR_ON_7TH=19
ROOK_OPEN_FILE=33
CONNECTED_MAJORS_ON_7TH=67
ROOK_HALF_OPEN_FILE=21
*/
static const int32_t rook_open_file                  =  33;
static const int32_t rook_half_open_file             =  21;


static const int32_t major_on_7th                    =  19;
static const int32_t connected_majors_on_7th         =  67;


static const int bishop_pst[64] = {
   0,0,0,1,0,0,0,0,-9,9,8,7,11,10,7,-1,-2,13,25,25,25,27,18,13,-3,19,21,38,34,26,22,5,-4,11,18,25,28,12,8,3,-8,7,9,15,9,8,5,7,-2,1,6,-7,-4,-2,14,-2,-8,-2,-27,-21,-20,-20,-5,-4
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
   -30,-30,-30,-30,-30,-30,-30,-30,-30,-26,-26,-30,-30,-26,-26,-30,-27,-19,-18,-24,-25,-16,-17,-29,-27,-18,-11,-15,-14,-7,-11,-28,-30,-22,-11,-9,-8,-2,-10,-31,-24,-20,-14,-16,-16,-8,-15,-33,-14,-17,-16,-36,-38,-26,-13,-8,-16,-6,-26,-54,-58,-59,-15,-21
};


static const int king_endgame_pst[64] = {
   -30,-30,-30,-30,-30,-30,-30,-30,-30,-22,-26,-30,-30,-25,-22,-30,-27,-14,-12,-20,-21,-8,-9,-29,-27,-12,-3,-7,-6,2,-4,-28,-30,-19,-4,-2,0,0,-10,-34,-28,-20,-14,-13,-13,-12,-21,-36,-18,-21,-20,-31,-27,-20,-24,-39,-16,-15,-25,-52,-60,-41,-35,-57
};

static const int knight_pst[64] = {
   -38,-30,-30,-30,-30,-30,-30,-31,-38,-35,-26,-21,-24,-31,-36,-36,-34,-18,-4,-1,-10,-13,-23,-33,-28,-7,1,9,1,7,-9,-23,-29,-22,-6,-7,-4,-9,-13,-28,-44,-27,-20,-9,-6,-15,-18,-41,-27,-28,-31,-19,-22,-24,-30,-29,-8,-41,-10,-35,-27,-30,-33,-8
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
   0,0,0,0,0,0,0,0,99,79,69,63,51,45,43,51,42,54,51,43,40,49,41,33,-7,-6,-7,3,16,17,3,-7,-22,-21,-18,-12,-6,-9,-13,-23,-23,-29,-22,-22,-10,-18,-9,-22,-26,-35,-32,-27,-28,-10,-5,-28,0,0,0,0,0,0,0,0
};

static const int pawn_endgame_pst[64] = {
   0,0,0,0,0,0,0,0,104,85,73,65,53,45,47,55,63,62,55,45,40,47,44,41,12,7,3,-6,-3,6,5,0,-6,-9,-20,-16,-13,-14,-12,-16,-17,-17,-19,-13,-6,-10,-16,-22,-9,-13,-13,-22,-16,3,-11,-24,0,0,0,0,0,0,0,0
};


static const int queen_pst[64] = {
   -1,7,7,9,13,7,7,7,-27,-40,-5,0,10,16,5,8,-15,-6,1,11,20,33,31,34,-14,-3,0,10,24,27,29,30,-14,-2,0,9,10,19,16,16,-13,-6,0,-4,-4,1,6,-1,-15,-11,-6,-10,-9,-10,-10,-4,-9,-19,-21,-10,-20,-23,-9,-5
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
   25,23,18,16,17,17,16,22,11,12,15,19,17,13,9,12,11,17,20,19,17,22,13,9,2,8,11,14,10,15,8,6,-14,-4,-6,-7,-9,-2,0,-6,-28,-16,-20,-21,-21,-15,-2,-14,-33,-24,-25,-28,-27,-14,-13,-19,-18,-17,-14,-13,-12,-7,-8,-25
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
 * \param mgscore       a pointer to the middle game score accumulator
 * \param egscore       a pointer to the endgame score accumulator
 *
 */
void eval_pawn(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore);


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
