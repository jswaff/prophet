#include "../../src/eval/eval_internal.h"

#include "prophet/position.h"

#include <gtest/gtest.h>

TEST(eval_test, pawn_doubled)
{
    position_t pos;

    ASSERT_TRUE(
        set_pos(&pos,"k7/p1p3p1/3p3p/1P5P/1PP1P1P1/8/8/K7 w - - 0 1"));

    /*
    k - - - - - - -
    p - p - - - p -
    - - - p - - - p    white to move
    - P - - - - - P    no ep
    - P P - P - P -    no castling rights
    - - - - - - - -
    - - - - - - - -
    K - - - - - - -
    */

    EXPECT_FALSE(pawn_doubled(&pos, A7));
    EXPECT_TRUE(pawn_doubled(&pos, B5));
    EXPECT_TRUE(pawn_doubled(&pos, B4));
    EXPECT_FALSE(pawn_doubled(&pos, C4));
    EXPECT_FALSE(pawn_doubled(&pos, C7));
    EXPECT_FALSE(pawn_doubled(&pos, D6));
    EXPECT_FALSE(pawn_doubled(&pos, E4));
    EXPECT_FALSE(pawn_doubled(&pos, G7));
    EXPECT_FALSE(pawn_doubled(&pos, G4));
    EXPECT_FALSE(pawn_doubled(&pos, H6));
    EXPECT_FALSE(pawn_isolated(&pos, H5));
}
