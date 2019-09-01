#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

TEST(eval_test, eval_major_on_7th)
{
    position_t pos;

    /* unconnected major on 7th (2nd) */
    ASSERT_TRUE(set_pos(&pos, "7k/8/8/8/8/8/r7/7K w - - 0 1"));

    EXPECT_EQ(major_on_7th, 
        eval_major_on_7th(&pos, A2));

    /* move the king out */
    ASSERT_TRUE(set_pos(&pos,"7k/8/8/8/8/7K/r7/8 w - - 0 1"));
    EXPECT_EQ(0, eval_major_on_7th(&pos, A2));
}

TEST(eval_test, eval_connected_major_on_7th)
{
    position_t pos;

    /* connected major on 7th (2nd) */
    ASSERT_TRUE(set_pos(&pos, "7k/2Q2R2/8/8/8/8/r7/7K w - - 0 1"));
    EXPECT_EQ(major_on_7th + connected_majors_on_7th,
        eval_major_on_7th(&pos, C7));

    /* the rook does not get the "connected" bonus */
    EXPECT_EQ(major_on_7th, eval_major_on_7th(&pos, F7));

}
