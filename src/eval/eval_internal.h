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
KING_SAFETY_MIDDLE_OPEN_FILE=-37
KING_SAFETY_PAWN_FAR_AWAY=-23
KING_SAFETY_PAWN_TWO_AWAY=-11
*/
static const int32_t king_safety_pawn_one_away       =  -2;
static const int32_t king_safety_pawn_two_away       = -11;
static const int32_t king_safety_pawn_far_away       = -23;
static const int32_t king_safety_middle_open_file    = -37;


/* pawn terms */
/*
DOUBLED_PAWN=-8
PASSED_PAWN=28
ISOLATED_PAWN=-12
*/
static const int32_t passed_pawn                     =  28;
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
MAJOR_ON_7TH=17
CONNECTED_MAJORS_ON_7TH=65
ROOK_HALF_OPEN_FILE=21
ROOK_OPEN_FILE=33
*/
static const int32_t rook_open_file                  =  33;
static const int32_t rook_half_open_file             =  21;


static const int32_t major_on_7th                    =  17;
static const int32_t connected_majors_on_7th         =  65;


static const int bishop_pst[64] = {
   0,0,0,1,0,0,0,0,-11,9,8,7,11,10,7,-1,-2,13,26,26,25,28,20,14,-5,19,21,40,34,27,20,5,-4,10,16,25,28,9,8,3,-8,7,7,13,7,7,6,8,-2,2,7,-8,-5,-2,17,-2,-8,-2,-28,-23,-22,-21,-5,-4
};

static const int bishop_endgame_pst[64] = {
   0,0,0,2,0,0,0,0,-9,11,8,8,11,10,7,-1,-2,14,25,25,25,29,18,13,-3,20,21,36,34,26,22,5,-4,11,20,25,28,14,8,3,-8,7,11,16,13,9,5,7,-2,-2,4,-4,-2,-3,8,-4,-8,-2,-27,-21,-20,-20,-5,-4
};

static const int king_pst[64] = {
   -30,-30,-30,-30,-30,-30,-30,-30,-30,-26,-26,-30,-30,-26,-26,-30,-27,-19,-18,-24,-25,-16,-17,-29,-27,-18,-11,-15,-14,-7,-11,-28,-30,-22,-11,-9,-8,-2,-10,-31,-24,-20,-14,-16,-16,-8,-15,-33,-14,-17,-16,-39,-42,-29,-11,-4,-18,-4,-28,-58,-59,-64,-12,-15
};

static const int king_endgame_pst[64] = {
   -30,-30,-30,-30,-30,-30,-30,-30,-30,-21,-26,-30,-30,-24,-20,-30,-27,-12,-10,-18,-19,-4,-5,-29,-27,-10,0,-4,-4,6,-2,-28,-30,-19,-2,0,2,0,-10,-34,-30,-20,-14,-11,-11,-13,-23,-38,-20,-23,-22,-31,-28,-20,-27,-44,-18,-19,-28,-54,-62,-40,-39,-65
};

static const int knight_pst[64] = {
   -40,-30,-30,-30,-30,-30,-30,-31,-38,-35,-25,-20,-24,-30,-36,-36,-34,-17,-2,1,-8,-11,-21,-33,-28,-5,3,11,2,11,-7,-21,-29,-22,-6,-8,-3,-8,-11,-26,-44,-26,-18,-9,-4,-13,-16,-39,-29,-30,-31,-17,-21,-23,-31,-29,-8,-41,-14,-35,-28,-29,-36,-8
};

static const int knight_endgame_pst[64] = {
   -38,-30,-30,-30,-30,-30,-30,-31,-38,-35,-26,-19,-23,-31,-36,-36,-34,-18,-2,-1,-9,-11,-23,-33,-28,-7,1,9,3,7,-9,-23,-29,-22,-4,-5,-3,-8,-13,-28,-46,-27,-21,-9,-8,-19,-20,-43,-29,-30,-33,-22,-24,-26,-32,-31,-10,-45,-12,-35,-29,-32,-35,-8
};

static const int pawn_pst[64] = {
   0,0,0,0,0,0,0,0,102,81,71,65,53,47,43,51,32,50,51,43,40,51,41,29,-14,-12,-11,4,17,20,1,-11,-28,-26,-20,-14,-6,-10,-13,-26,-28,-34,-26,-24,-12,-21,-9,-25,-32,-40,-36,-30,-31,-13,-4,-32,0,0,0,0,0,0,0,0
};

static const int pawn_endgame_pst[64] = {
   0,0,0,0,0,0,0,0,112,91,79,69,56,47,50,59,71,66,58,46,41,47,46,45,18,11,6,-9,-8,2,5,2,-1,-5,-19,-17,-15,-13,-12,-14,-13,-14,-17,-12,-6,-8,-17,-21,-3,-7,-7,-20,-10,4,-12,-23,0,0,0,0,0,0,0,0
};

static const int queen_pst[64] = {
   -1,7,7,10,15,8,7,9,-29,-44,-5,0,10,18,5,10,-17,-7,1,11,22,36,35,37,-16,-3,0,10,24,29,31,31,-15,-2,0,7,9,19,16,17,-14,-6,0,-5,-4,0,8,-1,-17,-12,-5,-9,-8,-9,-10,-4,-10,-21,-21,-10,-19,-25,-10,-5
};

static const int queen_endgame_pst[64] = {
   -1,7,9,11,15,9,7,9,-27,-38,-5,1,12,18,5,8,-15,-6,1,12,22,35,33,36,-14,-3,0,11,26,29,31,31,-14,-2,0,10,11,19,16,16,-13,-6,0,-4,-4,1,6,-1,-16,-11,-10,-12,-11,-12,-12,-4,-9,-19,-22,-14,-22,-25,-9,-5
};

static const int rook_pst[64] = {
   25,23,18,16,17,17,16,23,9,10,15,19,17,13,9,13,9,17,20,19,19,24,15,9,0,6,11,14,10,16,8,6,-16,-6,-8,-8,-11,-3,0,-6,-30,-18,-22,-23,-22,-17,-2,-15,-35,-26,-26,-30,-28,-14,-13,-21,-19,-18,-16,-12,-13,-8,-7,-25
};

static const int rook_endgame_pst[64] = {
   27,25,20,18,19,19,18,24,13,15,17,21,19,15,11,14,13,17,20,19,17,24,14,11,4,8,12,14,10,15,8,6,-13,-4,-6,-7,-9,-2,0,-6,-28,-16,-20,-21,-21,-15,-3,-14,-33,-24,-25,-28,-27,-16,-13,-19,-18,-17,-13,-15,-13,-6,-9,-25
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
