#ifndef _EVAL_INTERNAL_H_
#define _EVAL_INTERNAL_H_

#include <prophet/eval.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


/* king safety terms */
const int32_t king_safety_pawn_one_away          = -10;
const int32_t king_safety_pawn_two_away          = -20;
const int32_t king_safety_pawn_far_away          = -30;
const int32_t king_safety_middle_open_file       = -50;



// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _EVAL_INTERNAL_H_ */
