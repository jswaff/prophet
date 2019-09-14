#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

TEST(eval_test, eval_bishop)
{
    position_t pos;
    reset_pos(&pos);

    EXPECT_EQ(
        bishop_pst[C1], eval_bishop(&pos, C1));

    /* test the symmetry */
    EXPECT_EQ(eval_bishop(&pos, C1), eval_bishop(&pos, C8));
}
