#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

TEST(eval_test, eval_knight)
{
    position_t pos;
    reset_pos(&pos);

    EXPECT_EQ(
        knight_pst[B1] + knight_tropism * (int32_t)distance(B1, E8),
        eval_knight(&pos, B1, false));

    /* test the symmetry */
    EXPECT_EQ(eval_knight(&pos, B1, false), eval_knight(&pos, B8, false));
}
