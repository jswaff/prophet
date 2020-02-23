#ifndef _SEARCH_INTERNAL_H_
#define _SEARCH_INTERNAL_H_

#include <prophet/search.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

enum move_order_stage_t 
{ 
   GEN_CAPS, CAPTURES_PROMOS, GEN_NONCAPS, REMAINING 
};
typedef enum move_order_stage_t move_order_stage_t;

typedef struct 
{
    move_order_stage_t next_stage;
    move_t* start;
    move_t* end;
    move_t* current;
} move_order_dto;


/**
 * \brief Score a move using the MVV/LVA algorithm.
 *
 * \param mv            the chess move to score
 * 
 * \return the score
 */
int32_t mvvlva(move_t mv);


bool next(const position_t* pos, move_t** m, move_order_dto* mo);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _SEARCH_INTERNAL_H_ */
