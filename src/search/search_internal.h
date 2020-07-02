#ifndef _SEARCH_INTERNAL_H_
#define _SEARCH_INTERNAL_H_

#include <prophet/search.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

enum move_order_stage_t 
{ 
   PV, GEN_CAPS, CAPTURES_PROMOS, KILLER1, KILLER2, GEN_NONCAPS, REMAINING 
};
typedef enum move_order_stage_t move_order_stage_t;

typedef struct 
{
    move_order_stage_t next_stage;
    move_t* start;
    move_t* end;
    move_t* current;
    move_t pv_move;
    move_t killer1;
    move_t killer2;
} move_order_dto;


/**
 * \brief Score a move using the MVV/LVA algorithm.
 *
 * \param mv            the chess move to score
 * 
 * \return the score
 */
int32_t mvvlva(move_t mv);


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
 * \param killer1       killer move 1
 * \param killer2       killer move 2
 */
void initialize_move_ordering(move_order_dto* mo, move_t* stackptr,
    move_t pv_move, move_t killer1, move_t killer2);


/**
 * \brief Determine if the search should be stopped on time.
 *
 * \param opts          structure for tracking search options data
 * \param stats         structure for tracking search stats
 *
 * \return true if the search should be stopped, othwerwise false
 */
bool stop_search_on_time(search_options_t* opts, stats_t* stats);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _SEARCH_INTERNAL_H_ */
