#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

TEST(eval_test, eval_knight)
{
    position_t pos;
    reset_pos(&pos);

    /* test the piece square term */
    int32_t mg = 0; int32_t eg = 0;
    eval_knight(&pos, B1, &mg, &eg);
    EXPECT_EQ(knight_pst_mg[B1] + knight_tropism_mg * (int32_t)distance(B1, E8), mg);
    EXPECT_EQ(knight_pst_eg[B1] + knight_tropism_eg * (int32_t)distance(B1, E8), eg);

    /* test the symmetry */
    int32_t mg2 = 0; int32_t eg2 = 0;
    eval_knight(&pos, B8, &mg2, &eg2);
    EXPECT_EQ(mg, -mg2);
    EXPECT_EQ(eg, -eg2);
}
