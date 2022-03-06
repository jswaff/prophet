#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

TEST(eval_test, eval_bishop)
{
    position_t pos;
    reset_pos(&pos);

    EXPECT_EQ(
        bishop_pst[C1], eval_bishop(&pos, C1, false));

    /* test the symmetry */
    EXPECT_EQ(eval_bishop(&pos, C1, false), eval_bishop(&pos, C8, false));
}

TEST(eval_test, eval_bishop_endgame)
{
    position_t pos;
    reset_pos(&pos);

    EXPECT_EQ(
        bishop_endgame_pst[C1], eval_bishop(&pos, C1, true));

    /* test the symmetry */
    EXPECT_EQ(eval_bishop(&pos, C1, true), eval_bishop(&pos, C8, true));
}


TEST(eval_test, eval_bishop_pair)
{
    position_t pos;
    reset_pos(&pos);

    EXPECT_EQ(0, eval_bishop_pair(&pos));

    set_pos(&pos, "1rb1r1k1/2q2pb1/pp1p4/2n1pPPQ/Pn1BP3/1NN4R/1PP4P/R5K1 b - -");
    
    EXPECT_EQ(-bishop_pair, eval_bishop_pair(&pos));
}