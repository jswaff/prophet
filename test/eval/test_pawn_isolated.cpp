#include "eval/eval_internal.h"

#include "position/position.h"

#include <gtest/gtest.h>

TEST(eval_test, pawn_isolated)
{
    position_t pos;

    ASSERT_TRUE(
        set_pos(&pos,"k7/p1p3p1/3p3p/1P5P/1PP1P3/8/8/K7 b - - 0 1"));

    /*
    k - - - - - - -
    p - p - - - p -
    - - - p - - - p    black to move
    - P - - - - - P    no ep
    - P P - P - - -    no castling rights
    - - - - - - - -
    - - - - - - - -
    K - - - - - - -
    */

    EXPECT_TRUE(pawn_isolated(&pos, A7));
    EXPECT_FALSE(pawn_isolated(&pos, B5));
    EXPECT_FALSE(pawn_isolated(&pos, B4));
    EXPECT_FALSE(pawn_isolated(&pos, C4));
    EXPECT_FALSE(pawn_isolated(&pos, C7));
    EXPECT_FALSE(pawn_isolated(&pos, D6));
    EXPECT_TRUE(pawn_isolated(&pos, E4));
    EXPECT_FALSE(pawn_isolated(&pos, G7));
    EXPECT_FALSE(pawn_isolated(&pos, H6));
    EXPECT_TRUE(pawn_isolated(&pos, H5));
}
