#include "eval/eval_internal.h"

#include "prophet/position.h"

#include "parameters.h"
#include "position/position_internal.h"

#include <gtest/gtest.h>

#include <stdint.h>

// forward decls
static void dummy_eval_func(const position_t *pos, square_t sq, int32_t *mgscore, int32_t *egscore);


TEST(eval_test, eval_accumulator)
{
    position_t pos;
    reset_pos(&pos);

    // our dummy eval function always returns a value of 3 for mg and 5 for eg
    int32_t mg = 0;
    int32_t eg = 0;
    eval_accumulator(&pos, pos.black_bishops, &mg, &eg, &dummy_eval_func);
    EXPECT_EQ(6, mg);
    EXPECT_EQ(10, eg);
}


static void dummy_eval_func(const position_t* UNUSED(pos), square_t UNUSED(sq), int32_t *mgscore, 
	int32_t *egscore)
{
	*mgscore += 3;
	*egscore += 5;
}
