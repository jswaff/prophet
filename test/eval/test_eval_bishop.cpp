#include "../../src/eval/eval_internal.h"

#include "prophet/position.h"

#include <gtest/gtest.h>

#include <stdint.h>

TEST(eval_test, eval_bishop)
{
    position_t pos;
    reset_pos(&pos);

    /* test the piece square term */
    int32_t mg = 0; int32_t eg = 0;
    eval_bishop(&pos, C1, &mg, &eg);
    EXPECT_EQ(bishop_pst_mg[C1] + bishop_mobility_mg[0], mg);
    EXPECT_EQ(bishop_pst_eg[C1] + bishop_mobility_eg[0], eg);


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


TEST(eval_test, eval_bishop_mobility)
{
    position_t pos;
    set_pos(&pos, "r2qk2r/3bbppp/p1pppn2/6B1/3NP3/2N5/PPP2PPP/R2QR1K1 w - -");

    int32_t mg = 0; int32_t eg = 0;
    eval_bishop(&pos, G5, &mg, &eg);
    EXPECT_EQ(bishop_pst_mg[G5] + bishop_mobility_mg[6], mg);
    EXPECT_EQ(bishop_pst_eg[G5] + bishop_mobility_eg[6], eg);
}


TEST(eval_test, eval_bishop_trapped)
{
    position_t pos;
    set_pos(&pos, "8/pp2k1p1/4pp2/1P6/7p/P3P1P1/5PKb/2B5 b - - 0 1");

    int32_t mg = 0; int32_t eg = 0;
    eval_bishop(&pos, H2, &mg, &eg);
    EXPECT_EQ(-bishop_pst_mg[flip_rank[H2]] - bishop_mobility_mg[1] - bishop_trapped, mg);
    EXPECT_EQ(-bishop_pst_eg[flip_rank[H2]] - bishop_mobility_eg[1] - bishop_trapped, eg);
}
