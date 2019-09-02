#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

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

}
