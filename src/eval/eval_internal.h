#ifndef _EVAL_INTERNAL_H_
#define _EVAL_INTERNAL_H_

#include <prophet/bitmap.h>
#include <prophet/eval.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


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
static const int32_t connected_majors_on_7th         =  39;


static const int bishop_pst[64] = {
  -47,-28,1,-31,11,0,28,-42,-41,-42,-22,4,16,17,-38,-44,-4,-24,-20,2,31,54,54,5,-15,-16,1,17,13,-19,-22,11,-36,-2,-11,14,-3,-22,-6,-8,-19,-17,-7,-22,-9,2,3,-12,-5,-24,-18,-27,-12,0,-7,1,-62,-10,-53,-23,-14,-45,8,-66
};

static const int bishop_endgame_pst[64] = {
  -8,-8,8,-33,-12,12,7,6,-9,-3,-1,-8,0,-11,7,-15,-6,17,9,0,-5,4,10,0,-7,3,15,17,-6,20,1,-20,-10,3,18,6,15,12,-11,-38,7,-9,3,15,17,1,-33,-39,-8,-22,-34,-13,-18,-9,-22,-33,-50,-22,-37,-21,-33,-26,-39,-42
};


static const int king_pst[64] = {
  32,67,34,75,59,58,47,85,64,69,47,-15,1,37,39,-8,-1,32,-8,-35,-37,13,31,17,-61,-18,-24,-86,-68,-32,26,-11,-61,-18,-26,-52,-54,-53,-58,17,-14,23,25,-6,-63,-58,-37,-16,16,-13,8,-29,-45,-25,22,29,5,29,28,-99,-21,-45,43,54
};


static const int king_endgame_pst[64] = {
  -22,56,36,70,60,53,3,13,37,68,76,44,72,82,72,14,49,83,51,60,64,66,79,53,19,59,55,33,48,61,63,43,-8,41,31,39,38,40,41,18,-7,-13,19,12,30,30,18,6,-48,4,-13,15,12,12,-12,-21,-7,-34,-23,3,-31,0,-35,-62
};

static const int knight_pst[64] = {
  -111,-44,-2,12,-8,0,-27,-104,-25,-35,20,29,12,-35,-22,-99,6,17,27,34,59,62,48,7,1,9,31,56,7,26,-21,2,-12,19,3,-12,8,10,34,-18,-36,-40,-14,13,23,-7,3,-48,-55,-32,-15,-6,-12,-24,-16,7,-95,-23,-47,-48,-39,-32,-26,-110
};

static const int knight_endgame_pst[64] = {
  -55,-1,15,-9,-6,-11,-12,-46,-21,21,15,11,-10,-7,32,-48,-17,-6,26,8,14,19,3,-34,-8,9,19,24,23,29,3,-21,-28,1,11,11,-9,19,0,-25,-42,-9,-8,2,6,-20,-13,-31,-61,-50,-18,-20,-21,-38,-47,-25,-55,-48,-66,-42,-53,-51,-36,-84
};


static const int pawn_pst[64] = {
  0,0,0,0,0,0,0,0,149,130,141,149,64,99,77,90,40,18,43,56,72,52,35,20,-10,-14,-3,5,4,10,3,-11,-17,-20,-9,-3,1,2,-23,-27,-24,-18,-22,-23,-1,-6,-5,-9,-24,-30,-32,-33,-12,6,3,-27,0,0,0,0,0,0,0,0
};

static const int pawn_endgame_pst[64] = {
  0,0,0,0,0,0,0,0,127,108,107,109,87,94,126,86,90,71,82,50,37,48,69,50,40,20,13,10,7,0,18,18,17,8,-4,-16,-12,2,2,4,0,-6,-2,3,6,4,-4,-5,16,1,11,-7,-9,7,-12,-4,0,0,0,0,0,0,0,0
};


static const int queen_pst[64] = {
  1,61,79,68,52,125,114,94,-21,-9,12,61,77,76,53,80,-10,33,35,48,85,113,94,42,-23,9,21,32,36,43,38,30,-12,15,0,20,24,46,21,42,-10,3,-1,10,31,18,-2,6,-6,-2,8,4,10,24,34,33,-8,1,-6,-3,3,8,-15,31
};

static const int queen_endgame_pst[64] = {
  39,48,56,52,59,77,68,39,21,41,60,61,75,86,56,33,17,9,53,68,84,85,94,79,14,11,39,72,51,89,54,41,12,22,36,34,43,55,43,34,5,16,20,1,5,15,29,22,-38,2,-11,3,7,-2,-6,4,-25,-32,-14,-11,-35,-41,-30,37
};


static const int rook_pst[64] = {
  29,38,37,48,41,33,27,31,-8,-12,-8,13,14,35,41,39,-24,-12,12,30,29,37,36,36,-10,-28,-34,-4,-41,20,10,-25,-57,-8,-50,-36,-33,-38,-22,-13,-45,-57,-54,-47,-42,-44,-5,-25,-61,-41,-28,-47,-36,-24,-3,-41,-35,-33,-29,-25,-18,-26,-43,-37
};

static const int rook_endgame_pst[64] = {
  -8,1,0,10,6,2,-8,-6,11,17,19,11,2,5,2,1,6,8,2,1,3,0,-3,-2,-4,11,10,0,-7,-8,2,-8,-10,0,3,-9,2,6,-16,-14,-31,-5,-19,-8,-14,-10,-32,-38,-45,-29,-46,-26,-29,-21,-52,-33,-34,-29,-29,-19,-29,-24,-34,-50
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
