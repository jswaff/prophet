#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

TEST(eval_test, eval_bishop)
{
    position_t pos;
    reset_pos(&pos);

    /* test the piece square term */
    int32_t mg = 0; int32_t eg = 0;
    eval_bishop(&pos, C1, &mg, &eg);
    EXPECT_EQ(bishop_pst[C1], mg);
    EXPECT_EQ(bishop_endgame_pst[C1], eg);


    /* test the symmetry */
    int32_t mg2 = 0; int32_t eg2 = 0;
    eval_bishop(&pos, C8, &mg2, &eg2);
    EXPECT_EQ(mg, -mg2);
    EXPECT_EQ(eg, -eg2);
}


TEST(eval_test, eval_bishop_pair)
{
    position_t pos;
    reset_pos(&pos);

    EXPECT_EQ(0, eval_bishop_pair(&pos));

    set_pos(&pos, "1rb1r1k1/2q2pb1/pp1p4/2n1pPPQ/Pn1BP3/1NN4R/1PP4P/R5K1 b - -");
    
    EXPECT_EQ(-bishop_pair, eval_bishop_pair(&pos));
}