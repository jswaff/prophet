#include "eval/eval_internal.h"

#include "position/position.h"

#include <gtest/gtest.h>

TEST(eval_test, trapped_bishop_initial_pos)
{
    position_t pos;
    reset_pos(&pos);

    EXPECT_FALSE(trapped_bishop(&pos, C1, true));
    EXPECT_FALSE(trapped_bishop(&pos, F1, true));
    EXPECT_FALSE(trapped_bishop(&pos, C8, false));
    EXPECT_FALSE(trapped_bishop(&pos, F8, false));
}

TEST(eval_test, trapped_bishop_h2)
{
    position_t pos;

    ASSERT_TRUE(
        set_pos(&pos,"8/pp2k1p1/4pp2/1P6/7p/P3P1P1/5PKb/2B5 w - - 0 1"));

    EXPECT_TRUE(trapped_bishop(&pos, H2, false));
}

TEST(eval_test, trapped_bishop_a2)
{
    position_t pos;

    ASSERT_TRUE(
        set_pos(&pos,"rn1qkbnr/pppppppp/8/8/8/1P6/b1PPPPPP/2KR1BNR b - - 0 1"));

    EXPECT_TRUE(trapped_bishop(&pos, A2, false));
}

TEST(eval_test, trapped_bishop_a7)
{
    position_t pos;

    ASSERT_TRUE(
        set_pos(&pos,"rn1qkbnr/B1pppppp/1p6/8/8/1P6/b2PPPPP/2KR1BNR b - - 0 1"));

    EXPECT_TRUE(trapped_bishop(&pos, A7, true));
}

TEST(eval_test, trapped_bishop_h7)
{
    position_t pos;

    ASSERT_TRUE(
        set_pos(&pos,"rnbq1rk1/pppn1p1B/4p1p1/3pP3/1b1P4/2N2N2/PPP2PPP/R1BQK2R w - - 0 1"));

    EXPECT_TRUE(trapped_bishop(&pos, H7, true));
}

