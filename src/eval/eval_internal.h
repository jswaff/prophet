#pragma once

#include "position/position.h"

#include <stdbool.h>
#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

extern int32_t pawn_val;
extern int32_t knight_val;
extern int32_t bishop_val;
extern int32_t bishop_pair;
extern int32_t rook_val;
extern int32_t queen_val;
extern int32_t knight_kaufman_adj;
extern int32_t rook_kaufman_adj;

extern int32_t king_safety_pawn_one_away;
extern int32_t king_safety_wing_pawn_one_away;
extern int32_t king_safety_pawn_two_away;
extern int32_t king_safety_wing_pawn_two_away;
extern int32_t king_safety_pawn_far_away;
extern int32_t king_safety_wing_pawn_far_away;
extern int32_t king_safety_middle_open_file;

extern int32_t passed_pawn_mg[8];
extern int32_t passed_pawn_eg[8];
extern int32_t isolated_pawn_mg;
extern int32_t isolated_pawn_eg;
extern int32_t doubled_pawn_mg;
extern int32_t doubled_pawn_eg;

extern int32_t knight_tropism_mg;
extern int32_t knight_tropism_eg;
extern int32_t knight_outpost[64];
extern int32_t knight_supported_outpost[64];

extern int32_t bishop_mobility_mg[14];
extern int32_t bishop_mobility_eg[14];
extern int32_t bishop_trapped;

extern int32_t rook_mobility_mg[15];
extern int32_t rook_mobility_eg[15];
extern int32_t rook_open_file_mg;
extern int32_t rook_open_file_supported_mg;
extern int32_t rook_open_file_eg;
extern int32_t rook_open_file_supported_eg;
extern int32_t rook_half_open_file_mg;
extern int32_t rook_half_open_file_supported_mg;
extern int32_t rook_half_open_file_eg;
extern int32_t rook_half_open_file_supported_eg;

extern int32_t queen_mobility_mg[28];
extern int32_t queen_mobility_eg[28];

extern int32_t major_on_7th_mg;
extern int32_t major_on_7th_eg;
extern int32_t connected_majors_on_7th_mg;
extern int32_t connected_majors_on_7th_eg;

/* psts */
extern int32_t pawn_pst_mg[64];
extern int32_t pawn_pst_eg[64];
extern int32_t knight_pst_mg[64];
extern int32_t knight_pst_eg[64];
extern int32_t bishop_pst_mg[64];
extern int32_t bishop_pst_eg[64];
extern int32_t rook_pst_mg[64];
extern int32_t rook_pst_eg[64];
extern int32_t queen_pst_mg[64];
extern int32_t queen_pst_eg[64];
extern int32_t king_pst_mg[64];
extern int32_t king_pst_eg[64];


enum material_type_t { KK, KKN, KKB, KKNN, KPKN, KPKB, KNK, KNKP, KNKN, KNKB, 
                       KNNK, KBK, KBKP, KBKN, KBKB, OTHER };
typedef enum material_type_t material_type_t;


/* define a generic signature for piece type eval functions */
typedef void (*eval_func_t)(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore);


/**
 * @brief Evaluate a chess position for the side to move.
 *
 * Performs a static analysis of a chess position.  The score is primarily influenced by material counts, but it also 
 * takes into account several well known heuristics, such as king safety, passed pawns, isolated pawns, rooks on open 
 * files, and several others.  
 *
 * This method will not detect end-of-game scenarios such as checkmate.
 *
 * @param pos             a pointer to a chess position
 * @param material_only   if the evaluation should consider material only
 * @param use_pawn_hash   if the pawn hash table should be used
 *
 * @return the score
 */
int32_t eval(const position_t *pos, bool material_only, bool use_pawn_hash);


/**
 * @brief Evaluate a single bishop.
 *
 * @param pos           a pointer to a chess position
 * @param sq            the square the bishop is on
 * @param mgscore       a pointer to the middle game score accumulator
 * @param egscore       a pointer to the endgame score accumulator
 */
void eval_bishop(const position_t *pos, square_t sq, int32_t *mgscore, int32_t *egscore);


/**
 * @brief Evaluate the position for bishop pairs.
 *
 * @param pos           a pointer to a chess position
 *
 * @return a score for the bishop
 */
int32_t eval_bishop_pair(const position_t *pos);


/**
 * @brief Evaluate a single king.
 * 
 * @param pos           a pointer to a chess position
 * @param sq            the square the king is on
 * @param mgscore       a pointer to the middle game score accumulator
 * @param egscore       a pointer to the endgame score accumulator
 */
void eval_king(const position_t *pos, square_t sq, int32_t *mgscore, int32_t *egscore);


/**
 * @brief Evaluate king safety for one side of the board.
 *
 * @param pos           a pointer to a chess position
 * @param wtm           white to move?
 *
 * @return a score of the king safety for the player specified
 */
int32_t eval_king_safety(const position_t *pos, bool wtm);


/**
 * @brief Evaluate a single knight.
 *
 * @param pos           a pointer to a chess position
 * @param sq            the square the knight is on
 * @param mgscore       a pointer to the middle game score accumulator
 * @param egscore       a pointer to the endgame score accumulator
 */
void eval_knight(const position_t *pos, square_t sq, int32_t *mgscore, int32_t *egscore);


/**
 * @brief Evaluate a connected major on the 7th rank.
 *
 * A major piece is a rook or a queen.  For black, the "7th rank" is the "2nd rank".
 *
 * @param pos           a pointer to a chess position
 * @param sq            the square the major piece is on
 * @param mgscore       a pointer to the middle game score accumulator
 * @param egscore       a pointer to the endgame score accumulator
 */
void eval_major_on_7th(const position_t *pos, square_t sq, int32_t *mgscore, int32_t *egscore);


/**
 * @brief Evaluate the position's "material type".
 *
 * @param pos           a pointer to a chess position
 * @param draw_flag     a pointer to an integer to set the IMMEDIATE_DRAW flag
 *
 * @return the material type
 */
material_type_t eval_material_type(const position_t *pos, int *draw_flag);


/**
 * @brief Evaluate a single pawn.
 *
 * @param pos           a pointer to a chess position
 * @param sq            the square the pawn is on
 * @param mgscore       a pointer to the middle game score accumulator
 * @param egscore       a pointer to the endgame score accumulator
 */
void eval_pawn(const position_t *pos, square_t sq, int32_t *mgscore, int32_t *egscore);


/**
 * @brief Evaluate a single queen.
 *
 * @param pos           a pointer to a chess position
 * @param sq            the square the queen is on
 * @param mgscore       a pointer to the middle game score accumulator
 * @param egscore       a pointer to the endgame score accumulator
 */
void eval_queen(const position_t *pos, square_t sq, int32_t *mgscore, int32_t *egscore);


/**
 * @brief Evaluate a single rook.
 *
 * @param pos           a pointer to a chess position
 * @param sq            the square the rook is on
 * @param mgscore       a pointer to the middle game score accumulator
 * @param egscore       a pointer to the endgame score accumulator
 */
void eval_rook(const position_t *pos, square_t sq, int32_t *mgscore, int32_t *egscore);


/**
 * @brief Calculate a tapered score.
 *
 * Given a "middle game score", an "end game score", calculate a blended score in the range [mg, eg] depending on how 
 * much material is on the board.  Positions with the majority of major/minor pieces will be weighted heavily towards 
 * the middle game score.  Likewise, positions with few or no major/minor pieces will be weighted towards the end game
 * score.

 * @param pos           a pointer to a chess position
 * @param mg_score      the middle game score
 * @param eg_score      the end game score
 *
 * @return the tapered score
 */
int32_t eval_taper(const position_t *pos, int32_t mg_score, int32_t eg_score);


/**
 * @brief Determine if a square is an outpost.
 *
 * @param pos           a pointer to a chess position
 * @param pawn_sq       the square
 * @param white_pov     whether to examine from white's POV
 *
 * @return true if the square is an outpost, otherwise false
 */
bool outpost(const position_t *pos, square_t sq, bool white_pov);


/**
 * @brief Determine if a pawn is doubled.
 *
 * @param pos           a pointer to a chess position
 * @param pawn_sq       the pawn square
 *
 * @return true if the pawn is doubled, otherwise false
 */
bool pawn_doubled(const position_t *pos, square_t pawn_sq);


/**
 * @brief Determine if a pawn is isolated.
 *
 * An isolated pawn is a pawn that has no friendly pawn on an adjacent file.
 *
 * @param pos           a pointer to a chess position
 * @param pawn_sq       the pawn square
 *
 * @return true if the pawn is isolated, otherwise false
 */
bool pawn_isolated(const position_t *pos, square_t pawn_sq);


/**
 * @brief Determine if a pawn is passed.
 *
 * An passed pawn is a pawn that no enemy pawn can stop from queening.
 *
 * @param pos           a pointer to a chess position
 * @param pawn_sq       the pawn square
 *
 * @return true if the pawn is passed, otherwise false
 */
bool pawn_passed(const position_t *pos, square_t pawn_sq);


/**
 * @brief Determine if a pawn supports a square.
 *
 * @param pos           a pointer to a chess position
 * @param sq            the square in question
 *
 * @return true if a friendly pawn supports, otherwise false
 */ 
bool pawn_supports(const position_t *pos, square_t sq);


/**
 * @brief Determine if bishop on square would be trapped.
 *
 * @param pos           a pointer to a chess position
 * @param pawn_sq       the square
 * @param is_white      whether to examine from white's POV
 *
 * @return true if the bishop would be trapped, otherwise false
 */
bool trapped_bishop(const position_t *pos, square_t sq, bool is_white);


/**
 * @brief Accumulate the score for a set of pieces.
 *
 * @param pos           a pointer to a chess position
 * @param piece_map     a set of pieces to evaluate
 * @param mgscore       a pointer to the middle game score accumulator
 * @param egscore       a pointer to the endgame score accumulator
 * @param eval_func     the evaluation function to use for each piece
 */
void eval_accumulator(const position_t *pos, uint64_t piece_map, int32_t *mgscore, int32_t *egscore, 
    eval_func_t eval_func);


/**
 * @brief Evaluate the position's non-pawn material.
 *
 * @param pos           a pointer to a chess position
 * @param for_white     whether the returned value should be for white
 *
 * @return the score
 */
int32_t eval_nonpawn_material(const position_t *pos, bool for_white);


/**
 * @brief Evaluate the position's pawn material.
 *
 * @param pos           a pointer to a chess position
 * @param for_white     whether the returned value should be for white
 *
 * @return the score
 */
int32_t eval_pawn_material(const position_t *pos, bool for_white);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus
