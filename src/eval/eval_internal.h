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
static const int32_t king_safety_middle_open_file    = -77;


/* pawn terms */
static const int32_t passed_pawn                     =  40;
static const int32_t isolated_pawn                   = -13;
static const int32_t doubled_pawn                    =  -3;


/* knight terms */
static const int32_t knight_tropism                  =  -3;

/* bishop terms */


/* rook terms */
static const int32_t rook_open_file                  =  22;
static const int32_t rook_half_open_file             =  10;


static const int32_t major_on_7th                    =  19;
static const int32_t connected_majors_on_7th         =  49;


/* bishop piece square table */
static const int bishop_pst[64] = {
   -12, -2, 17,-22,  6, 14, 16, -4,
   -12, -7,  4,  5, 14,  3, -1,-24,
     3, 14,  9, 12, 17, 34, 35, 16,
    -1,  4, 22, 24, 11, 11, -1,  7,
   -11, 11, 14, 19, 16,  2,  0, -6,
     2, -2,  8,  2, 12, 12,  2,-14,
     4, -9,-15,-10, -1,  5, -2,  3,
   -31, -9,-31,-15,-14,-28,-10,-31  
};


/* king piece square table for non-endgame.
 * the idea is to encourage safety. */
static const int king_pst[64] = {
    -6,  1,  0,  1,  1,  1,-18,  1,
     1,  1,  1,-21, -4,  0,  1,-23,
    -6,  1,-10,-16,-14, -1,  1, -3,
   -38, -3,-15,-46,-38,-14,  0, -5,
   -42,-16,-31,-27,-29,-38,-25, -4,
   -26, -5,  1,-12,-33,-33,-30,-24,
   -26,-17, -8,-26,-35,-23, 12, 15,
     7, 22, 24,-56,-20,-48, 36, 29  
};


/* king piece square table for endgame.
 * this table encourages centrality. */
static const int king_endgame_pst[64] = {
   -17, 31, 30, 31, 31, 31, 23, 14,
    31, 41, 41, 35, 41, 41, 41, 12,
    31, 41, 48, 49, 50, 51, 41, 31,
    16, 41, 49, 19, 36, 48, 41, 31,
   -13, 37, 26, 28, 26, 30, 28, 17,
   -11, -9, 18,  6, 18, 17, 13,  5,
   -31,  2,-14, 13,  5,  8,-10,-22,
   -12,-22,-14,-16,-27,  1,-24,-31  
};


/* knight piece square table */
static const int knight_pst[64] = {
   -36, -6, 18,  7,  5,  4,-11,-36,
   -15,  4, 27, 26,  5, -5, 15,-36,
     2, 13, 35, 29, 41, 42, 31, -6,
     5, 20, 34, 46, 23, 36, -3,  1,
    -7, 18, 17,  6, 11, 23, 25,-10,
   -28,-23, -3, 17, 23,  1,  6,-33,
   -36,-31, -9,  0, -4,-25,-22,  6,
   -41,-16,-36,-36,-36,-33,-18,-41  
};


/* pawn piece square table */
static const int pawn_pst[64] = {
     0,  0,  0,  0,  0,  0,  0,  0,
    61, 61, 61, 61, 61, 61, 61, 61,
    45, 43, 45, 42, 41, 43, 45, 33,
    13, -1, -1,  1,  2, -1,  9,  4,
    -4,-10, -8,-11, -8, -2,-11, -13,
   -15,-14,-16,-18, -1, -5, -7, -11,
    -9,-20,-20,-29,-13,  3, -5, -19,
     0,  0,  0,  0,  0,  0,  0,   0  
};


/* queen piece square table */
static const int queen_pst[64] = {
     2, 30, 30, 30, 30, 30, 30, 30,
   -29,-13, 11, 31, 31, 31, 31, 30,
   -19,  4, 23, 32, 32, 32, 31, 30,
   -27, -7, 11, 30, 25, 32, 29, 15,
   -26, -1, -6, 10, 12, 30, 11, 20,
   -24,-12,-12, -8,  5, -1,-12,-11,
   -32,-19,-13,-15,-10, -5, -2,  2,
   -32,-27,-25,-22,-25,-29,-32, 11  
};


/* rook piece square table */
static const int rook_pst[64] = {
    13, 22, 20, 30, 25, 17,  6, 12,
    10, 15, 17, 18, 13, 21, 20, 18,
     7, 15, 16, 25, 22, 21, 17, 19,
     5,  9,  3, 10, -8, 10, 14, -5,
   -19,  8, -9, -9, -3, -2,-10, -5,
   -26,-18,-25,-14,-17,-15,-10,-20,
   -36,-22,-22,-25,-21,-12,-19,-26,
   -24,-19,-15,-11, -9,-13,-30,-31  
};


enum material_type_t { KK, KKN, KKB, KKNN, KPKN, KPKB, KNK, KNKP, KNKN, KNKB, 
                       KNNK, KBK, KBKP, KBKN, KBKB, OTHER };
typedef enum material_type_t material_type_t;


/* define a generic signature for piece type eval functions */
typedef int32_t (*eval_func_t)(const position_t* pos, square_t sq);


/**
 * \brief Evaluate a single bishop.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the bishop is on
 *
 * \return a score for the bishop.
 */
int32_t eval_bishop(const position_t* pos, square_t sq);


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
 *
 * \return a score for the knight.
 */
int32_t eval_knight(const position_t* pos, square_t sq);


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
 *
 * \return a score for the pawn.
 */
int32_t eval_pawn(const position_t* pos, square_t sq);


/**
 * \brief Evaluate a single queen.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the queen is on
 *
 * \return a score for the queen.
 */
int32_t eval_queen(const position_t* pos, square_t sq);


/**
 * \brief Evaluate a single rook.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the rook is on
 *
 * \return a score for the rook.
 */
int32_t eval_rook(const position_t* pos, square_t sq);


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
 * \param eval_func     the evaluation function to use for each piece
 *
 * \return the cumulative score.
 */
int32_t eval_accumulator(const position_t* pos, uint64_t piece_map, 
  eval_func_t eval_func);



// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _EVAL_INTERNAL_H_ */
