#pragma once

#include "prophet/search.h"

#include "position/position.h"

#include <stdbool.h>
#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

/* piece values for move ordering purposes (not material evaluation) */
static const int32_t see_pawn_val   =  100;
static const int32_t see_knight_val =  300;
static const int32_t see_bishop_val =  300;
static const int32_t see_rook_val   =  500;
static const int32_t see_queen_val  = 1000;

enum move_order_stage_t { 
   PV, HASH_MOVE, GEN_CAPS, GOOD_CAPTURES_PROMOS, KILLER1, KILLER2, 
   GEN_NONCAPS, NONCAPS, INIT_BAD_CAPTURES, BAD_CAPTURES
};
typedef enum move_order_stage_t move_order_stage_t;


typedef struct {
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


typedef struct {
    pv_func_t pv_callback;
    uint64_t start_time;
    uint64_t stop_time;
    uint64_t nodes_between_time_checks;
    uint64_t node_count_last_time_check;
} search_options_t;


typedef struct {
    bool early_exit_ok;
    uint32_t max_depth;
    uint32_t max_time_ms;
    pv_func_t pv_callback;
    bool print_summary;
} iterator_options_t;


typedef struct {
    position_t* pos;
    move_t* move_stack;
    undo_t* undo_stack;
} iterator_context_t;


/**
 * @brief Determine the next move to play.
 *
 * @param pos           the chess position 
 * @param m             pointer to a move pointer to be set to the selected
 *                      move
 * @param mo            the move ordering context
 *
 * @return true if a move has been selected, or false if there are no further moves
 */
bool next(const position_t *pos, move_t **m, move_order_dto *mo);


/**
 * @brief Initialize move ordering.
 *
 * @param mo            pointer to the move ordering data structure
 * @param stackptr      pointer to move stack to place new moves on
 * @param pv_move       pv move
 * @param hash_move     hash move
 * @param killer1       killer move 1
 * @param killer2       killer move 2
 * @param gen_noncaps   whether to generate non-capturing moves
 * @param play_badcaps  whether to play bad captures 
 */
void initialize_move_ordering(move_order_dto *mo, move_t *stackptr, move_t pv_move, move_t hash_move, move_t killer1, 
    move_t killer2, bool gen_noncaps, bool play_badcaps);


/**
 * @brief Score a move using the MVV/LVA algorithm.
 *
 * @param mv            the chess move to score
 * 
 * @return the score
 */
int32_t mvvlva(move_t mv);


/**
 * @brief Evaluate a piece for move ordering purposes.
 *
 * Note- this method should not be used for a material evaluation.
 *
 * @param piece         the piece to evaluate
 *
 * @return the score
 */
int32_t see_eval_piece(int32_t piece);


/**
 * @brief Score a move using static exchange analysis (SEE).
 *
 * @param pos           the chess position
 * @param mv            the chess move to score
 * 
 * @return the score
 */
int32_t see(const position_t *pos, move_t mv);


/**
 * @brief Search the position to until it is "quiet".
 *
 * Quiescence search - attempt to obtain a score by searching until the position is quiet.
 *
 * @param pos           a pointer to a chess position
 * @param alpha         the lower bound
 * @param beta          the upper bound
 * @param move_stack    pre-allocated stack for move generation
 * @param undo_stack    pre-allocated stack for undo information
 * @param stats         structure for tracking search stats
 * @param opts          structure for tracking search options data
 * 
 * @return the score
 */
int32_t qsearch(position_t *pos, int32_t alpha, int32_t beta, move_t *move_stack, undo_t *undo_stack, stats_t *stats, 
    search_options_t *opts);


/**
 * @brief Search the position to a fixed depth.
 *
 * @param pos           a pointer to a chess position
 * @param parent_pv     a pointer to the move line that will receive the PV
 * @param depth         the depth to search to
 * @param alpha         the lower bound
 * @param beta          the upper bound
 * @param move_stack    pre-allocated stack for move generation
 * @param undo_stack    pre-allocated stack for undo information
 * @param stats         structure for tracking search stats
 * @param opts          structure for tracking search options data
 * 
 * @return the score
 */
int32_t search(position_t *pos, move_line_t *parent_pv, int32_t depth, int32_t alpha, int32_t beta, move_t *move_stack,
    undo_t *undo_stack, stats_t *stats, search_options_t *opts);


/**
 * @brief Search the position using iterative deepening. 
 * 
 * @param depth         pointer to variable to record depth achieved by search
 * @param score         pointer to variable to record score returned by search
 * @param opts          the options structure
 * @param ctx           the context for this search iterator
 * @param stats         a stats structure to record the search statistics
 *
 * @return the principal variation
 */ 
move_line_t iterate(uint32_t *depth, int32_t *score, const iterator_options_t *opts, const iterator_context_t *ctx, 
    stats_t *stats);


/**
 * @brief Determine if the search should be stopped on time.
 *
 * @param opts          structure for tracking search options data
 * @param stats         structure for tracking search stats
 *
 * @return true if the search should be stopped, othwerwise false
 */
bool stop_search_on_time(search_options_t *opts, const stats_t *stats);


/**
 * @brief - Determine if a position is zugzwang.
 *
 * @param pos           a pointer to a chess position
 *
 * @return - true if the position is zugzwang, otherwise false
 */
bool zugzwang(const position_t *pos);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus
