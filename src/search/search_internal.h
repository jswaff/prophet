#pragma once

#include <prophet/search.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

/* piece values for move ordering purposes (not material evaluation) */
static const int32_t see_pawn_val = 100;
static const int32_t see_knight_val = 300;
static const int32_t see_bishop_val = 300;
static const int32_t see_rook_val = 500;
static const int32_t see_queen_val = 1000;

enum move_order_stage_t 
{ 
   PV, HASH_MOVE, GEN_CAPS, GOOD_CAPTURES_PROMOS, KILLER1, KILLER2, 
   GEN_NONCAPS, NONCAPS, INIT_BAD_CAPTURES, BAD_CAPTURES
};
typedef enum move_order_stage_t move_order_stage_t;

typedef struct 
{
    move_order_stage_t next_stage;
    move_t* start;
    move_t* end;
    move_t* current;
    move_t pv_move;
    move_t hash_move;
    move_t killer1;
    move_t killer2;
    bool gen_noncaps;
    bool play_badcaps;
} move_order_dto;


/**
 * \brief Determine the next move to play.
 *
 * \param pos           the chess position 
 * \param m             pointer to a move pointer to be set to the selected
 *                      move
 * \param mo            the move ordering context
 *
 * \return true if a move has been selected, or false if there are no
 *    further moves.
 */
bool next(const position_t* pos, move_t** m, move_order_dto* mo);


/**
 * \brief Initialize move ordering
 *
 * \param mo            pointer to the move ordering data structure
 * \param stackptr      pointer to move stack to place new moves on
 * \param pv_move       pv move
 * \param hash_move     hash move
 * \param killer1       killer move 1
 * \param killer2       killer move 2
 * \param gen_noncaps   whether to generate non-capturing moves
 * \param play_badcaps  whether to play bad captures 
 */
void initialize_move_ordering(move_order_dto* mo, move_t* stackptr,
    move_t pv_move, move_t hash_move, move_t killer1, move_t killer2, 
    bool gen_noncaps, bool play_badcaps);


/**
 * \brief Evaluate a piece for move ordering purposes.
 *
 * Note- this method should not be used for a material evaluation.
 *
 * \param piece         the piece to evaluate
 *
 * \return the score
 */
int32_t see_eval_piece(int32_t piece);


/**
 * \brief Determine if the search should be stopped on time.
 *
 * \param opts          structure for tracking search options data
 * \param stats         structure for tracking search stats
 *
 * \return true if the search should be stopped, othwerwise false
 */
bool stop_search_on_time(search_options_t* opts, const stats_t* stats);


/**
 * \brief - Determine if a position is zugzwang
 *
 * \param pos           a pointer to a chess position
 *
 * \return - true if the position is zugzwang, otherwise false.
 */
bool zugzwang(const position_t* pos);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus
