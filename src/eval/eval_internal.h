#ifndef _EVAL_INTERNAL_H_
#define _EVAL_INTERNAL_H_

#include <prophet/bitmap.h>
#include <prophet/eval.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

/*
ROOK_PST=44,53,54,74,56,57,49,61,-9,-14,-7,12,14,38,64,62,-27,-13,14,31,33,48,50,56,-11,-32,-38,-4,-41,22,9,-25,-58,-8,-53,-39,-34,-46,-23,-12,-45,-59,-55,-49,-43,-45,0,-23,-62,-41,-28,-48,-36,-24,5,-42,-35,-33,-29,-25,-18,-26,-43,-37
QUEEN_ENDGAME_PST=45,48,54,49,59,65,54,10,34,68,87,64,78,89,59,18,20,9,55,73,82,85,96,100,32,12,41,84,50,113,58,41,21,22,49,40,45,55,44,30,4,16,20,1,4,13,37,27,-42,2,-11,3,7,-8,-16,1,-27,-35,-14,-11,-36,-63,-35,37
KING_PST=66,115,60,124,84,91,88,132,109,94,54,-18,-26,32,40,3,-9,1,-13,-55,-76,11,25,11,-75,-29,-29,-126,-118,-44,23,-17,-65,-22,-29,-57,-57,-58,-77,12,-13,27,28,-5,-70,-60,-37,-14,28,-18,8,-29,-45,-25,23,29,1,28,28,-100,-21,-46,43,54
BISHOP_ENDGAME_PST=-8,-9,9,-33,-13,12,6,12,-5,-1,-2,-7,0,-13,12,-13,-6,17,10,1,-8,-10,5,0,-6,5,15,17,-6,20,1,-20,-6,4,19,6,16,12,-11,-38,8,-10,3,16,17,1,-33,-39,-9,-22,-34,-13,-18,-9,-22,-36,-50,-22,-37,-21,-33,-26,-51,-44
KING_SAFETY_PAWN_FAR_AWAY=-31
KING_SAFETY_PAWN_TWO_AWAY=-31
QUEEN_PST=-3,59,81,73,56,152,137,129,-26,-23,-9,59,78,75,51,92,-12,33,34,46,86,112,93,32,-30,10,19,26,35,33,36,30,-14,15,-5,17,23,45,21,42,-10,3,-1,11,30,18,-6,5,-5,-2,8,4,10,27,38,34,-8,1,-6,-3,3,13,-15,31
PAWN_ENDGAME_PST=0,0,0,0,0,0,0,0,125,106,103,102,89,96,133,85,92,77,82,49,34,49,68,50,40,20,13,10,7,0,18,18,17,8,-4,-16,-12,2,2,4,0,-6,-2,3,6,4,-4,-5,16,1,12,-7,-9,7,-12,-4,0,0,0,0,0,0,0,0
KNIGHT_ENDGAME_PST=-27,8,15,-9,-1,-9,-10,-22,-21,42,14,10,-11,-3,39,-36,-19,-7,27,7,8,18,1,-34,-9,10,18,24,23,28,3,-21,-28,1,11,11,-9,19,0,-25,-42,-8,-8,2,6,-20,-12,-29,-61,-51,-17,-20,-21,-39,-47,-30,-51,-50,-68,-44,-53,-51,-35,-84
MAJOR_ON_7TH=7
ROOK_ENDGAME_PST=-11,-2,-4,2,3,-4,-13,-12,12,19,19,11,2,6,-3,-5,7,9,1,0,2,-2,-5,-7,-4,14,12,0,-6,-8,4,-8,-9,0,6,-6,6,12,-14,-13,-30,-2,-18,-5,-12,-9,-34,-38,-43,-28,-46,-23,-29,-20,-55,-32,-34,-29,-29,-19,-29,-24,-33,-50
ROOK_OPEN_FILE=24
KNIGHT_PST=-148,-86,-2,14,-16,-2,-40,-155,-23,-86,20,31,14,-44,-48,-133,9,20,25,34,77,70,53,7,2,9,31,56,7,28,-21,3,-11,17,3,-12,7,11,35,-18,-35,-39,-13,12,23,-7,2,-49,-54,-33,-15,-6,-12,-23,-17,8,-103,-22,-46,-48,-39,-30,-25,-105
KING_ENDGAME_PST=-29,50,37,60,58,48,-2,14,28,62,74,43,76,81,71,14,48,88,51,62,70,66,79,53,19,59,55,39,56,62,64,43,-8,42,31,40,38,41,46,18,-7,-13,19,12,32,30,18,6,-53,7,-13,15,12,12,-12,-21,-5,-35,-24,3,-31,0,-35,-62
KNIGHT_TROPISM=-3
CONNECTED_MAJORS_ON_7TH=38
DOUBLED_PAWN=-4
PASSED_PAWN=32
ROOK_HALF_OPEN_FILE=14
KING_SAFETY_PAWN_ONE_AWAY=2
KING_SAFETY_MIDDLE_OPEN_FILE=-81
BISHOP_PST=-55,-37,-6,-33,23,-11,44,-85,-48,-46,-23,4,13,21,-53,-47,-5,-24,-22,1,40,103,71,7,-17,-15,0,17,13,-19,-22,12,-38,0,-11,14,-3,-22,-10,-7,-20,-16,-7,-22,-9,2,2,-13,-5,-24,-19,-27,-12,1,-7,4,-60,-10,-53,-23,-14,-45,51,-65
PAWN_PST=0,0,0,0,0,0,0,0,159,138,160,180,56,96,43,92,30,-2,41,58,82,50,33,19,-11,-15,-3,5,4,10,3,-11,-17,-20,-9,-3,1,2,-23,-27,-24,-18,-22,-23,-1,-6,-5,-9,-24,-30,-32,-33,-12,6,3,-27,0,0,0,0,0,0,0,0
ISOLATED_PAWN=-13
*/


/* king safety terms */
static const int32_t king_safety_pawn_one_away       =   2;
static const int32_t king_safety_pawn_two_away       = -31;
static const int32_t king_safety_pawn_far_away       = -31;
static const int32_t king_safety_middle_open_file    = -81;


/* pawn terms */
static const int32_t passed_pawn                     =  32;
static const int32_t isolated_pawn                   = -13;
static const int32_t doubled_pawn                    =  -4;


/* knight terms */
static const int32_t knight_tropism                  =  -3;

/* bishop terms */


/* rook terms */
static const int32_t rook_open_file                  =  24;
static const int32_t rook_half_open_file             =  14;


static const int32_t major_on_7th                    =   7;
static const int32_t connected_majors_on_7th         =  38;


static const int bishop_pst[64] = {
   -55,-37, -6,-33, 23,-11, 44,-85,
   -48,-46,-23,  4, 13, 21,-53,-47,
    -5,-24,-22,  1, 40,103, 71,  7,
   -17,-15,  0, 17, 13,-19,-22, 12,
   -38,  0,-11, 14, -3,-22,-10, -7,
   -20,-16, -7,-22, -9,  2,  2,-13,
    -5,-24,-19,-27,-12,  1, -7,  4,
   -60,-10,-53,-23,-14,-45, 51,-65
};

static const int bishop_endgame_pst[64] = {
    -8, -9,  9,-33,-13, 12,  6, 12,
    -5, -1, -2, -7,  0,-13, 12,-13,
    -6, 17, 10,  1, -8,-10,  5,  0,
    -6,  5, 15, 17, -6, 20,  1,-20,
    -6,  4, 19,  6, 16, 12,-11,-38,
     8,-10,  3, 16, 17,  1,-33,-39,
    -9,-22,-34,-13,-18, -9,-22,-36,
   -50,-22,-37,-21,-33,-26,-51,-44
};


static const int king_pst[64] = {
    66, 115,  60, 124,  84,  91,  88, 132,
   109,  94,  54, -18, -26,  32,  40,   3,
    -9,   1, -13, -55, -76,  11,  25,  11,
   -75, -29, -29,-126,-118, -44,  23, -17,
   -65, -22, -29, -57, -57, -58, -77,  12,
   -13,  27,  28,  -5, -70, -60, -37, -14,
    28, -18,   8, -29, -45, -25,  23,  29,
     1,  28,  28,-100, -21, -46,  43,  54
};


static const int king_endgame_pst[64] = {
   -29, 50, 37, 60, 58, 48, -2, 14,
    28, 62, 74, 43, 76, 81, 71, 14,
    48, 88, 51, 62, 70, 66, 79, 53,
    19, 59, 55, 39, 56, 62, 64, 43,
    -8, 42, 31, 40, 38, 41, 46, 18,
    -7,-13, 19, 12, 32, 30, 18,  6,
   -53,  7,-13, 15, 12, 12,-12,-21,
    -5,-35,-24,  3,-31,  0,-35,-62
};

static const int knight_pst[64] = {
   -148, -86,  -2,  14, -16,  -2, -40,-155,
    -23, -86,  20,  31,  14, -44, -48,-133,
      9,  20,  25,  34,  77,  70,  53,   7,
      2,   9,  31,  56,   7,  28, -21,   3,
    -11,  17,   3, -12,   7,  11,  35, -18,
    -35, -39, -13,  12,  23,  -7,   2, -49,
    -54, -33, -15,  -6, -12, -23, -17,   8,
   -103, -22, -46, -48, -39, -30, -25,-105
};

static const int knight_endgame_pst[64] = {
   -27,  8, 15, -9, -1, -9,-10,-22,
   -21, 42, 14, 10,-11, -3, 39,-36,
   -19, -7, 27,  7,  8, 18,  1,-34,
    -9, 10, 18, 24, 23, 28,  3,-21,
   -28,  1, 11, 11, -9, 19,  0,-25,
   -42, -8, -8,  2,  6,-20,-12,-29,
   -61,-51,-17,-20,-21,-39,-47,-30,
   -51,-50,-68,-44,-53,-51,-35,-84
};


static const int pawn_pst[64] = {
     0,  0,  0,  0,  0,  0,  0,  0,
   159,138,160,180, 56, 96, 43, 92,
    30, -2, 41, 58, 82, 50, 33, 19,
   -11,-15, -3,  5,  4, 10,  3,-11,
   -17,-20, -9, -3,  1,  2,-23,-27,
   -24,-18,-22,-23, -1, -6, -5, -9,
   -24,-30,-32,-33,-12,  6,  3,-27,
     0,  0,  0,  0,  0,  0,  0,  0
};

static const int pawn_endgame_pst[64] = {
     0,  0,  0,  0,  0,  0,  0,  0,
   125,106,103,102, 89, 96,133, 85,
    92, 77, 82, 49, 34, 49, 68, 50,
    40, 20, 13, 10,  7,  0, 18, 18,
    17,  8, -4,-16,-12,  2,  2,  4,
     0, -6, -2,  3,  6,  4, -4, -5,
    16,  1, 12, -7, -9,  7,-12, -4,
     0,  0,  0,  0,  0,  0,  0,  0
};


static const int queen_pst[64] = {
    -3, 59, 81, 73, 56,152,137,129,
   -26,-23, -9, 59, 78, 75, 51, 92,
   -12, 33, 34, 46, 86,112, 93, 32,
   -30, 10, 19, 26, 35, 33, 36, 30,
   -14, 15, -5, 17, 23, 45, 21, 42,
   -10,  3, -1, 11, 30, 18, -6,  5,
    -5, -2,  8,  4, 10, 27, 38, 34,
    -8,  1, -6, -3,  3, 13,-15, 31
};

static const int queen_endgame_pst[64] = {
    45, 48, 54, 49, 59, 65, 54, 10,
    34, 68, 87, 64, 78, 89, 59, 18,
    20,  9, 55, 73, 82, 85, 96,100,
    32, 12, 41, 84, 50,113, 58, 41,
    21, 22, 49, 40, 45, 55, 44, 30,
     4, 16, 20,  1,  4, 13, 37, 27,
   -42,  2,-11,  3,  7, -8,-16,  1,
   -27,-35,-14,-11,-36,-63,-35, 37
};

static const int rook_pst[64] = {
    44, 53, 54, 74, 56, 57, 49, 61,
    -9,-14, -7, 12, 14, 38, 64, 62,
   -27,-13, 14, 31, 33, 48, 50, 56,
   -11,-32,-38, -4,-41, 22,  9,-25,
   -58, -8,-53,-39,-34,-46,-23,-12,
   -45,-59,-55,-49,-43,-45,  0,-23,
   -62,-41,-28,-48,-36,-24,  5,-42,
   -35,-33,-29,-25,-18,-26,-43,-37
};

static const int rook_endgame_pst[64] = {
   -11, -2, -4,  2,  3, -4,-13,-12,
    12, 19, 19, 11,  2,  6, -3, -5,
     7,  9,  1,  0,  2, -2, -5, -7,
    -4, 14, 12,  0, -6, -8,  4, -8,
    -9,  0,  6, -6,  6, 12,-14,-13,
   -30, -2,-18, -5,-12, -9,-34,-38,
   -43,-28,-46,-23,-29,-20,-55,-32,
   -34,-29,-29,-19,-29,-24,-33,-50
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
