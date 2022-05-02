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
KING_SAFETY_PAWN_ONE_AWAY=-2
KING_SAFETY_MIDDLE_OPEN_FILE=-34
KING_SAFETY_PAWN_FAR_AWAY=-22
KING_SAFETY_PAWN_TWO_AWAY=-10
*/
static const int32_t king_safety_pawn_one_away       =  -2;
static const int32_t king_safety_pawn_two_away       = -10;
static const int32_t king_safety_pawn_far_away       = -22;
static const int32_t king_safety_middle_open_file    = -34;


/* pawn terms */
/*
DOUBLED_PAWN=-8
PASSED_PAWN=29
ISOLATED_PAWN=-12
*/
static const int32_t passed_pawn                     =  29;
static const int32_t isolated_pawn                   = -12;
static const int32_t doubled_pawn                    =  -8;


/* knight terms */
/*
KNIGHT_TROPISM=-8
*/
static const int32_t knight_tropism                  =  -8;

/* bishop terms */


/* rook terms */
/*
MAJOR_ON_7TH=18
ROOK_OPEN_FILE=33
CONNECTED_MAJORS_ON_7TH=66
ROOK_HALF_OPEN_FILE=20
*/
static const int32_t rook_open_file                  =  33;
static const int32_t rook_half_open_file             =  20;


static const int32_t major_on_7th                    =  18;
static const int32_t connected_majors_on_7th         =  66;


static const int bishop_pst[64] = {
   0,0,0,1,0,0,0,0,-10,9,8,7,11,10,7,-1,-2,13,26,26,25,27,19,13,-4,19,21,39,34,26,21,5,-4,11,17,25,28,10,8,3,-8,7,8,14,8,7,6,8,-2,2,7,-8,-4,-2,16,-2,-8,-2,-28,-22,-21,-20,-5,-4
};

static const int bishop_endgame_pst[64] = {
   0,0,0,2,0,0,0,0,-9,10,8,8,11,10,7,-1,-2,14,25,25,25,28,18,13,-3,19,21,37,34,26,22,5,-4,11,19,25,28,13,8,3,-8,7,10,16,11,8,5,7,-2,-1,5,-5,-3,-3,11,-3,-8,-2,-27,-21,-20,-20,-5,-4
};

static const int king_pst[64] = {
   -30,-30,-30,-30,-30,-30,-30,-30,-30,-26,-26,-30,-30,-26,-26,-30,-27,-19,-18,-24,-25,-16,-17,-29,-27,-18,-11,-15,-14,-7,-11,-28,-30,-22,-11,-9,-8,-2,-10,-31,-24,-20,-14,-16,-16,-8,-15,-33,-14,-17,-16,-37,-40,-27,-12,-6,-17,-5,-27,-56,-59,-62,-13,-18
};

static const int king_endgame_pst[64] = {
   -30,-30,-30,-30,-30,-30,-30,-30,-30,-22,-26,-30,-30,-24,-21,-30,-27,-13,-11,-19,-20,-6,-7,-29,-27,-11,-1,-5,-5,4,-3,-28,-30,-19,-3,-1,1,0,-10,-34,-29,-20,-14,-12,-12,-12,-22,-37,-19,-22,-21,-31,-28,-20,-25,-42,-17,-17,-27,-53,-61,-41,-37,-61
};

static const int knight_pst[64] = {
   -39,-30,-30,-30,-30,-30,-30,-31,-38,-35,-26,-20,-24,-30,-36,-36,-34,-18,-3,0,-9,-12,-22,-33,-28,-6,2,10,1,9,-8,-22,-29,-22,-6,-7,-4,-8,-12,-27,-44,-26,-19,-9,-4,-14,-17,-40,-28,-29,-31,-18,-21,-24,-31,-29,-8,-41,-12,-35,-27,-30,-35,-8
};

static const int knight_endgame_pst[64] = {
   -38,-30,-30,-30,-30,-30,-30,-31,-38,-35,-26,-20,-23,-31,-36,-36,-34,-18,-3,-1,-10,-12,-23,-33,-28,-7,1,9,2,7,-9,-23,-29,-22,-5,-6,-3,-9,-13,-28,-45,-27,-21,-9,-7,-17,-19,-42,-28,-29,-32,-21,-23,-25,-31,-30,-9,-43,-11,-35,-28,-31,-34,-8
};

static const int pawn_pst[64] = {
   0,0,0,0,0,0,0,0,101,80,70,64,52,46,43,51,37,52,51,43,40,50,41,31,-11,-9,-9,4,17,19,2,-9,-25,-24,-19,-13,-6,-9,-13,-25,-26,-31,-24,-23,-12,-20,-9,-24,-29,-38,-35,-29,-30,-11,-5,-30,0,0,0,0,0,0,0,0
};

static const int pawn_endgame_pst[64] = {
   0,0,0,0,0,0,0,0,108,88,76,67,55,46,49,57,67,64,57,46,40,47,45,43,15,9,5,-8,-6,4,5,1,-3,-7,-20,-17,-15,-14,-12,-15,-15,-15,-18,-12,-6,-9,-16,-22,-6,-10,-10,-21,-13,4,-12,-24,0,0,0,0,0,0,0,0
};

static const int queen_pst[64] = {
   -1,7,7,10,14,7,7,8,-28,-42,-5,0,10,17,5,9,-16,-7,1,11,21,34,33,35,-15,-3,0,10,24,28,30,31,-15,-2,0,8,9,19,16,17,-14,-6,0,-5,-4,1,7,-1,-16,-11,-6,-9,-8,-10,-10,-4,-10,-20,-21,-10,-20,-24,-10,-5
};

static const int queen_endgame_pst[64] = {
   -1,7,8,10,14,8,7,8,-27,-39,-5,1,11,17,5,8,-15,-6,1,11,21,34,32,35,-14,-3,0,11,25,28,30,31,-14,-2,0,10,10,19,16,16,-13,-6,0,-4,-4,1,6,-1,-16,-11,-8,-11,-10,-11,-11,-4,-9,-19,-22,-12,-21,-24,-9,-5
};

static const int rook_pst[64] = {
   25,23,18,16,17,17,16,23,10,11,15,19,17,13,9,12,10,17,20,19,18,23,14,9,1,7,11,14,10,16,8,6,-15,-5,-7,-8,-10,-3,0,-6,-29,-17,-21,-22,-22,-16,-2,-15,-34,-25,-26,-29,-28,-14,-13,-20,-19,-17,-15,-12,-13,-7,-8,-25
};

static const int rook_endgame_pst[64] = {
   26,24,19,17,18,18,17,23,12,14,16,20,18,14,10,13,12,17,20,19,17,23,14,10,3,8,12,14,10,15,8,6,-13,-4,-6,-7,-9,-2,0,-6,-28,-16,-20,-21,-21,-15,-2,-14,-33,-24,-25,-28,-27,-15,-13,-19,-18,-17,-13,-14,-13,-6,-9,-25
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
 * \param mgscore       a pointer to the middle game score accumulator
 * \param egscore       a pointer to the endgame score accumulator
 *
 */
void eval_bishop(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore);


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
 * \param mgscore       a pointer to the middle game score accumulator
 * \param egscore       a pointer to the endgame score accumulator
 *
 */
void eval_knight(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore);


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
 * \param mgscore       a pointer to the middle game score accumulator
 * \param egscore       a pointer to the endgame score accumulator
 *
 */
void eval_queen(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore);


/**
 * \brief Evaluate a single rook.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the rook is on
 * \param mgscore       a pointer to the middle game score accumulator
 * \param egscore       a pointer to the endgame score accumulator
 *
 */
void eval_rook(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore);


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
