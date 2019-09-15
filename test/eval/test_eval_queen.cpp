#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

TEST(eval_test, eval_queen)
{
    position_t pos;
    reset_pos(&pos);

    EXPECT_EQ(
        queen_pst[D1], eval_queen(&pos, D1));

    /* test the symmetry */
    EXPECT_EQ(eval_queen(&pos, D1), eval_queen(&pos, D8));
}

TEST(eval_test, eval_queen_7th_king_back_rank)
{
    position_t pos;
    ASSERT_TRUE(set_pos(&pos, "7k/2Q2R2/8/8/8/8/r7/7K w - - 0 1"));
    
    EXPECT_EQ(queen_pst[C7] + major_on_7th + connected_majors_on_7th,
        eval_queen(&pos, C7));
}
