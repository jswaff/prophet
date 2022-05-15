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
KING_SAFETY_WING_PAWN_ONE_AWAY=0
KING_SAFETY_PAWN_TWO_AWAY=-14
KING_SAFETY_WING_PAWN_TWO_AWAY=-9
KING_SAFETY_PAWN_FAR_AWAY=-23
KING_SAFETY_WING_PAWN_FAR_AWAY=-24
KING_SAFETY_MIDDLE_OPEN_FILE=-39
*/
static const int32_t king_safety_pawn_one_away       =  -4;
static const int32_t king_safety_wing_pawn_one_away  =   0;
static const int32_t king_safety_pawn_two_away       = -14;
static const int32_t king_safety_wing_pawn_two_away  =  -9;
static const int32_t king_safety_pawn_far_away       = -23;
static const int32_t king_safety_wing_pawn_far_away  = -24;
static const int32_t king_safety_middle_open_file    = -39;


/* pawn terms */
/*
DOUBLED_PAWN=-7
PASSED_PAWN=30
ISOLATED_PAWN=-13
*/
static const int32_t passed_pawn                     =  30;
static const int32_t isolated_pawn                   = -13;
static const int32_t doubled_pawn                    =  -7;


/* knight terms */
/*
KNIGHT_TROPISM=-7
*/
static const int32_t knight_tropism                  =  -7;

/* bishop terms */
/*
BISHOP_MOBILITY=2
BISHOP_ENDGAME_MOBILITY=1
*/
static const int32_t bishop_mobility                 =   2;
static const int32_t bishop_endgame_mobility         =   1;


/* rook terms */
/*
ROOK_OPEN_FILE=33
MAJOR_ON_7TH=17
CONNECTED_MAJORS_ON_7TH=64
ROOK_HALF_OPEN_FILE=20
*/
static const int32_t rook_open_file                  =  33;
static const int32_t rook_half_open_file             =  20;


static const int32_t major_on_7th                    =  17;
static const int32_t connected_majors_on_7th         =  64;


static const int bishop_pst[64] = {
   0,0,0,1,0,0,0,0,-12,9,8,7,11,10,7,-1,-2,13,26,26,25,28,21,14,-6,19,21,41,34,27,20,5,-4,10,15,24,29,9,8,3,-8,7,6,12,7,7,6,8,-2,2,7,-8,-6,-2,17,-2,-8,-2,-28,-23,-23,-21,-5,-4
};

static const int bishop_endgame_pst[64] = {
   0,0,0,2,0,0,0,0,-9,12,8,8,11,10,7,-1,-2,14,25,25,25,30,18,13,-3,21,21,35,34,26,23,5,-4,11,21,25,28,15,8,3,-8,7,12,17,14,9,5,7,-2,-3,3,-3,-1,-4,6,-5,-8,-2,-27,-21,-20,-20,-5,-4
};

static const int king_pst[64] = {
   -30,-30,-30,-30,-30,-30,-30,-30,-30,-26,-26,-30,-30,-26,-26,-30,-27,-19,-18,-24,-25,-16,-17,-29,-27,-18,-11,-15,-14,-7,-11,-28,-30,-22,-11,-9,-8,-2,-10,-31,-24,-20,-14,-16,-16,-8,-15,-33,-14,-17,-16,-40,-44,-30,-10,-2,-19,-4,-29,-60,-59,-67,-10,-13
};

static const int king_endgame_pst[64] = {
   -30,-30,-30,-30,-30,-30,-30,-30,-30,-20,-26,-30,-30,-23,-19,-30,-27,-11,-9,-17,-18,-2,-3,-29,-27,-9,1,-3,-3,7,-1,-28,-30,-18,-1,1,3,0,-10,-34,-31,-20,-14,-11,-11,-13,-23,-39,-21,-24,-23,-31,-28,-20,-28,-46,-19,-21,-29,-55,-63,-40,-41,-69
};

static const int knight_pst[64] = {
   -41,-30,-30,-30,-30,-30,-30,-31,-38,-35,-24,-20,-24,-30,-36,-36,-34,-16,-1,2,-6,-10,-20,-33,-27,-4,4,12,2,13,-6,-20,-29,-22,-6,-8,-3,-7,-10,-25,-43,-26,-18,-8,-3,-13,-16,-38,-30,-31,-31,-17,-21,-22,-31,-29,-8,-41,-15,-35,-28,-29,-37,-8
};

static const int knight_endgame_pst[64] = {
   -38,-30,-30,-30,-30,-30,-30,-31,-38,-35,-26,-18,-23,-31,-36,-36,-34,-18,-1,-1,-9,-10,-23,-33,-28,-7,1,9,4,7,-9,-23,-29,-22,-3,-4,-3,-7,-13,-28,-47,-27,-21,-9,-9,-21,-21,-44,-30,-31,-34,-24,-25,-27,-33,-32,-10,-46,-13,-35,-30,-33,-36,-8
};

static const int pawn_pst[64] = {
   0,0,0,0,0,0,0,0,103,82,72,66,54,47,43,51,27,48,50,43,40,52,41,27,-17,-14,-12,4,17,21,1,-13,-30,-28,-22,-15,-7,-10,-13,-28,-31,-36,-27,-25,-13,-22,-9,-27,-35,-43,-38,-31,-32,-13,-5,-33,0,0,0,0,0,0,0,0
};

static const int pawn_endgame_pst[64] = {
   0,0,0,0,0,0,0,0,115,94,81,71,58,48,52,61,74,68,59,47,41,47,47,47,21,13,7,-10,-9,1,5,2,1,-4,-18,-17,-15,-13,-12,-14,-11,-12,-16,-12,-6,-7,-17,-20,0,-6,-5,-19,-8,4,-13,-23,0,0,0,0,0,0,0,0
};

static const int queen_pst[64] = {
   -1,7,7,10,16,8,7,10,-30,-45,-5,0,11,19,5,11,-17,-7,1,11,23,38,37,38,-17,-3,0,10,24,30,32,32,-15,-2,0,6,8,19,16,17,-15,-6,0,-5,-4,0,8,-1,-18,-12,-5,-9,-8,-9,-10,-4,-10,-22,-21,-9,-19,-26,-10,-5
};

static const int queen_endgame_pst[64] = {
   -1,7,10,12,16,10,7,9,-27,-37,-5,1,13,19,5,8,-15,-6,1,12,23,36,34,37,-14,-3,0,12,27,30,32,32,-14,-2,0,11,11,19,16,16,-13,-6,0,-4,-4,1,6,-1,-16,-11,-11,-13,-12,-13,-13,-4,-9,-19,-23,-16,-23,-26,-9,-5
};

static const int rook_pst[64] = {
   25,23,18,16,17,17,16,24,8,9,15,19,17,14,9,13,8,17,20,19,20,25,16,9,-1,5,11,14,10,16,8,6,-17,-7,-9,-9,-12,-4,0,-6,-31,-19,-23,-24,-23,-18,-2,-16,-36,-27,-27,-30,-28,-14,-13,-22,-19,-18,-16,-12,-13,-9,-7,-24
};

static const int rook_endgame_pst[64] = {
   28,26,21,19,20,20,19,25,14,16,18,22,20,16,12,15,13,17,20,19,17,25,14,12,4,9,12,14,10,15,8,6,-13,-4,-6,-7,-9,-2,0,-6,-28,-16,-20,-21,-21,-15,-3,-14,-33,-24,-25,-28,-27,-17,-13,-19,-18,-17,-12,-15,-13,-5,-10,-25
};

enum material_type_t { KK, KKN, KKB, KKNN, KPKN, KPKB, KNK, KNKP, KNKN, KNKB, 
                       KNNK, KBK, KBKP, KBKN, KBKB, OTHER };
typedef enum material_type_t material_type_t;


/* define a generic signature for piece type eval functions */
typedef void (*eval_func_t)(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore);


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
 * \param pos           a pointer to a chess position
 * \param sq            the square the king is on
 * \param mgscore       a pointer to the middle game score accumulator
 * \param egscore       a pointer to the endgame score accumulator
 *
 */
void eval_king(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore);


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
 * \param mgscore       a pointer to the middle game score accumulator
 * \param egscore       a pointer to the endgame score accumulator
 * \param eval_func     the evaluation function to use for each piece
 *
 */
void eval_accumulator(const position_t* pos, uint64_t piece_map, 
  int32_t* mgscore, int32_t* egscore, eval_func_t eval_func);



// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _EVAL_INTERNAL_H_ */
