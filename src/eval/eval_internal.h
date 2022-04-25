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
KING_SAFETY_PAWN_ONE_AWAY=-4
KING_SAFETY_MIDDLE_OPEN_FILE=-20
KING_SAFETY_PAWN_FAR_AWAY=-9
KING_SAFETY_PAWN_TWO_AWAY=-8
*/
static const int32_t king_safety_pawn_one_away       =  -4;
static const int32_t king_safety_pawn_two_away       =  -8;
static const int32_t king_safety_pawn_far_away       =  -9;
static const int32_t king_safety_middle_open_file    = -20;


/* pawn terms */
/*
ISOLATED_PAWN=-11
DOUBLED_PAWN=-9
PASSED_PAWN=30
*/
static const int32_t passed_pawn                     =  30;
static const int32_t isolated_pawn                   = -11;
static const int32_t doubled_pawn                    =  -9;


/* knight terms */
/*
KNIGHT_TROPISM=-11
*/
static const int32_t knight_tropism                  = -11;

/* bishop terms */


/* rook terms */
/*
ROOK_OPEN_FILE=34
MAJOR_ON_7TH=25
CONNECTED_MAJORS_ON_7TH=74
ROOK_HALF_OPEN_FILE=20
*/
static const int32_t rook_open_file                  =  34;
static const int32_t rook_half_open_file             =  20;


static const int32_t major_on_7th                    =  25;
static const int32_t connected_majors_on_7th         =  74;


static const int bishop_pst[64] = {
   0,0,0,0,0,0,0,0,-4,7,8,7,8,8,7,-1,-2,10,20,20,20,21,13,7,-1,17,19,33,31,22,19,4,-2,9,18,25,27,13,8,3,-5,8,11,16,11,8,7,5,-1,3,7,-5,-3,2,13,-1,-4,-2,-24,-14,-13,-20,-3,-3
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
   -30,-30,-30,-30,-30,-30,-30,-30,-30,-27,-27,-30,-30,-27,-27,-30,-28,-21,-21,-25,-26,-19,-20,-30,-28,-21,-15,-18,-17,-11,-15,-28,-30,-24,-15,-11,-10,-4,-13,-30,-23,-20,-15,-16,-16,-8,-14,-29,-13,-15,-14,-28,-29,-17,-13,-18,-9,-2,-12,-41,-50,-42,-18,-32
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
   -33,-30,-30,-30,-30,-30,-30,-31,-33,-33,-30,-26,-28,-31,-33,-33,-32,-24,-16,-15,-21,-23,-28,-33,-29,-9,-8,-2,-5,-7,-16,-28,-29,-27,-12,-12,-10,-14,-22,-29,-35,-25,-20,-12,-10,-15,-20,-33,-17,-18,-24,-16,-18,-19,-21,-21,-3,-29,7,-30,-15,-28,-13,-3
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
   0,0,0,0,0,0,0,0,69,57,51,48,41,37,37,42,49,47,42,39,35,38,32,33,6,4,3,3,12,16,10,1,-11,-10,-13,-9,-3,-4,-5,-13,-14,-17,-14,-14,-5,-8,-5,-15,-14,-20,-19,-19,-18,0,-2,-21,0,0,0,0,0,0,0,0
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
   -1,2,2,3,5,2,2,2,-17,-26,-4,0,4,6,1,2,-8,-3,1,6,10,15,13,17,-7,-1,1,6,14,15,15,19,-7,0,1,8,8,13,10,9,-7,-3,2,-1,0,3,5,-1,-7,-7,-3,-7,-6,-6,-6,-3,-4,-10,-14,-7,-13,-12,-4,-4
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
   15,13,9,8,7,7,6,9,5,6,8,9,8,5,3,5,6,12,13,13,10,12,7,2,1,6,9,10,9,9,3,1,-10,-2,-2,-3,-4,-1,0,-5,-17,-8,-11,-12,-12,-8,0,-9,-20,-13,-16,-19,-18,-8,-7,-12,-13,-11,-9,-8,-7,0,-4,-24
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
