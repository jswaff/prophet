#include <prophet/parameters.h>

#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

// forward decls
static int32_t dummy_eval_func(const position_t* pos, square_t sq);


TEST(eval_test, eval_accumulator)
{
    position_t pos;
    reset_pos(&pos);

    // our dummy eval function always returns a value of 3
    EXPECT_EQ(6, eval_accumulator(&pos, pos.black_bishops, &dummy_eval_func));
    EXPECT_EQ(3, eval_accumulator(&pos, pos.white_queens, &dummy_eval_func));
    EXPECT_EQ(24, eval_accumulator(&pos, pos.white_pawns, &dummy_eval_func));
    EXPECT_EQ(0, eval_accumulator(&pos, 0, &dummy_eval_func));
}


static int32_t dummy_eval_func(const position_t* UNUSED(pos), square_t UNUSED(sq))
{
    return 3;
}
