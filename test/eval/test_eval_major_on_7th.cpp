#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

TEST(eval_test, eval_major_on_7th)
{
    position_t pos;

    /* unconnected black major on 7th (2nd) */
    ASSERT_TRUE(set_pos(&pos, "7k/8/8/8/8/8/r7/7K w - - 0 1"));

    int32_t mg = 0; int32_t eg = 0;
    eval_major_on_7th(&pos, A2, &mg, &eg);
    EXPECT_EQ(-major_on_7th_mg, mg);
    EXPECT_EQ(-major_on_7th_eg, eg);

    /* move the king out */
    ASSERT_TRUE(set_pos(&pos,"7k/8/8/8/8/7K/r7/8 w - - 0 1"));
    int32_t mg2 = 0; int32_t eg2 = 0;
    eval_major_on_7th(&pos, A2, &mg2, &eg2);
    EXPECT_EQ(0, mg2);
    EXPECT_EQ(0, eg2);
}

TEST(eval_test, eval_connected_major_on_7th)
{
    position_t pos;

    /* connected white major on 7th */
    ASSERT_TRUE(set_pos(&pos, "7k/2Q2R2/8/8/8/8/r7/7K w - - 0 1"));
    int32_t mg = 0; int32_t eg = 0;
    eval_major_on_7th(&pos, C7, &mg, &eg);
    EXPECT_EQ(major_on_7th_mg + connected_majors_on_7th_mg, mg);
    EXPECT_EQ(major_on_7th_eg + connected_majors_on_7th_eg, eg);

    /* the rook does not get the "connected" bonus */
    int32_t mg2= 0; int32_t eg2= 0;
    eval_major_on_7th(&pos, F7, &mg2, &eg2);
    EXPECT_EQ(major_on_7th_mg, mg2);
    EXPECT_EQ(major_on_7th_eg, eg2);
}
