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
KING_SAFETY_PAWN_ONE_AWAY=-7
KING_SAFETY_MIDDLE_OPEN_FILE=-14
KING_SAFETY_PAWN_FAR_AWAY=-6
KING_SAFETY_PAWN_TWO_AWAY=-6
*/
static const int32_t king_safety_pawn_one_away       =  -7;
static const int32_t king_safety_pawn_two_away       =  -6;
static const int32_t king_safety_pawn_far_away       =  -6;
static const int32_t king_safety_middle_open_file    = -14;


/* pawn terms */
/*
ISOLATED_PAWN=-13
DOUBLED_PAWN=-7
PASSED_PAWN=35
*/
static const int32_t passed_pawn                     =  35;
static const int32_t isolated_pawn                   = -13;
static const int32_t doubled_pawn                    =  -7;


/* knight terms */
/*
KNIGHT_TROPISM=-6
*/
static const int32_t knight_tropism                  =  -6;

/* bishop terms */


/* rook terms */
/*
ROOK_OPEN_FILE=42
CONNECTED_MAJORS_ON_7TH=77
ROOK_HALF_OPEN_FILE=30
MAJOR_ON_7TH=34
*/
static const int32_t rook_open_file                  =  42;
static const int32_t rook_half_open_file             =  30;


static const int32_t major_on_7th                    =  34;
static const int32_t connected_majors_on_7th         =  77;


static const int bishop_pst[64] = {
   -3,-1,-4,-2,-1,-2,-2,-2,-4,7,6,4,9,8,7,-7,-2,11,18,19,17,21,11,1,-1,12,22,32,32,22,12,3,-1,15,24,34,31,16,6,-1,-1,13,22,24,23,22,13,1,-2,6,10,2,10,9,26,-5,-10,-2,-22,-8,-5,-19,-4,-6
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
   -31,-31,-31,-30,-30,-29,-29,-30,-29,-24,-23,-24,-26,-23,-23,-29,-26,-20,-19,-23,-24,-12,-11,-25,-29,-18,-13,-15,-12,-6,-10,-30,-34,-27,-11,-13,-10,-4,-19,-38,-27,-20,-12,-15,-10,-8,-13,-34,-24,-17,-12,-31,-29,-12,-12,-29,-21,-16,-15,-50,-39,-44,-18,-43
};


static const int king_endgame_pst[64] = {
   0,0,0,0,0,0,1,1,1,13,11,12,11,13,13,1,3,14,26,22,21,27,19,2,0,14,25,29,30,28,20,0,-3,8,25,29,30,29,13,-4,-3,8,20,23,26,25,17,-6,-5,6,11,9,15,12,10,-12,-7,-9,-9,-5,-16,-8,-25,-25
};

static const int knight_pst[64] = {
   -39,-34,-31,-36,-33,-38,-34,-38,-37,-35,-26,-27,-34,-32,-37,-37,-37,-24,-11,-9,-19,-19,-24,-36,-33,-14,-5,9,12,-6,-12,-30,-33,-26,-5,1,6,-6,-24,-33,-37,-22,-3,-6,-9,6,-10,-36,-20,-19,-23,-12,-11,-12,-18,-18,-6,-37,-1,-34,-17,-31,-32,-4
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
   0,0,0,0,0,0,0,0,102,90,71,65,56,56,58,72,72,70,57,46,41,49,55,54,10,5,0,3,1,4,4,-2,-18,-18,-21,-12,-10,-21,-18,-27,-23,-22,-22,-25,-18,-18,-13,-26,-27,-24,-33,-44,-33,-6,-9,-32,0,0,0,0,0,0,0,0
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
   -7,4,7,6,8,5,2,5,-17,-24,-2,2,5,7,5,3,-9,-3,4,12,13,15,10,12,-8,-5,4,10,18,18,11,10,-9,-2,4,10,14,12,14,5,-5,-1,7,5,7,11,7,1,-10,-5,14,4,12,4,-3,-1,-4,-11,-8,13,-5,-12,-3,-6
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
   18,11,9,13,9,8,6,8,7,10,7,5,-1,9,4,5,6,11,8,9,3,6,6,-3,1,2,6,3,3,7,-2,1,-7,0,1,-1,-1,-1,0,-8,-13,-2,-7,-6,-7,-4,0,-11,-19,-3,-7,-5,-6,2,-4,-25,-5,0,6,9,10,17,-21,-21
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
