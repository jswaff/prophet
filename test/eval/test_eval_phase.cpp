#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

TEST(eval_test, eval_phase)
{
    position_t pos;
    reset_pos(&pos);

    EXPECT_EQ(0, eval_phase(&pos));

    set_pos(&pos, "k7/8/8/8/8/8/8/K7 w - -");
    EXPECT_EQ(24, eval_phase(&pos));

    set_pos(&pos, "kq6/8/8/8/8/8/8/KR6 b - -");
    EXPECT_EQ(18, eval_phase(&pos));
}
