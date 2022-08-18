#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

TEST(eval_test, outpost)
{
    position_t pos;

    ASSERT_TRUE(
        set_pos(&pos,"1r3rk1/3q1ppp/3p4/p1pNpP2/PpP1P1P1/1P3Q2/6KP/5R2 w - - 0 1"));

    EXPECT_TRUE(outpost(&pos, D5, true));
}

TEST(eval_test, outpost2)
{
    position_t pos;

    ASSERT_TRUE(
        set_pos(&pos,"r1br1k2/pp3pp1/1b4np/4P3/2pNN3/2P3B1/PP1R1PPP/3R2K1 w - - 0 1"));

    EXPECT_TRUE(outpost(&pos, D4, true));
    EXPECT_FALSE(outpost(&pos, E4, true));
    EXPECT_FALSE(outpost(&pos, G6, false));
}

