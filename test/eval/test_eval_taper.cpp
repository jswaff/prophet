#include "eval/eval_internal.h"

#include "prophet/position.h"

#include <gtest/gtest.h>

#include <stdint.h>

TEST(eval_test, eval_taper)
{
    position_t pos;
    reset_pos(&pos);

    int32_t mg_score = 100;
    int32_t eg_score = 200;

    EXPECT_EQ(100, eval_taper(&pos, mg_score, eg_score));

    set_pos(&pos, "k7/8/8/8/8/8/8/K7 w - -");
    EXPECT_EQ(200, eval_taper(&pos, mg_score, eg_score));

    set_pos(&pos, "kq6/8/8/8/8/8/8/KR6 b - -");
    EXPECT_EQ(175, eval_taper(&pos, mg_score, eg_score));
}
