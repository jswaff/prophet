#include "eval/eval_internal.h"

#include "prophet/position.h"

#include <gtest/gtest.h>

#include <stdint.h>

TEST(eval_test, eval_queen)
{
    position_t pos;
    reset_pos(&pos);

    int32_t mg = 0; int32_t eg = 0;
    eval_queen(&pos, D1, &mg, &eg);
    EXPECT_EQ(queen_pst_mg[D1] + queen_mobility_mg[0], mg);
    EXPECT_EQ(queen_pst_eg[D1] + queen_mobility_eg[0], eg);


    /* test the symmetry */
    int32_t mg2 = 0; int32_t eg2 = 0;
    eval_queen(&pos, D8, &mg2, &eg2);
    EXPECT_EQ(mg, -mg2);
    EXPECT_EQ(eg, -eg2);
}


TEST(eval_test, eval_queen_7th_king_back_rank)
{
    position_t pos;
    ASSERT_TRUE(set_pos(&pos, "7k/2Q2R2/8/8/8/8/r7/7K w - - 0 1"));
    
    int32_t mg = 0; int32_t eg = 0;
    eval_queen(&pos, C7, &mg, &eg);
    EXPECT_EQ(queen_pst_mg[C7] + major_on_7th_mg + connected_majors_on_7th_mg + queen_mobility_mg[20], mg);
    EXPECT_EQ(queen_pst_eg[C7] + major_on_7th_eg + connected_majors_on_7th_eg + queen_mobility_eg[20], eg);
}
