#include "eval/eval_internal.h"

#include "position/position.h"

#include <gtest/gtest.h>

TEST(eval_test, pawn_passed_wiki1)
{
    position_t pos;

    ASSERT_TRUE(
        set_pos(&pos,"7k/8/7p/1P2Pp1P/2Pp1PP1/8/8/7K w - - 0 1"));

    /*
    - - - - - - - k
    - - - - - - - -
    - - - - - - - p    white to move
    - P - - P p - P    no ep
    - - P p - P P -    no castling rights
    - - - - - - - -
    - - - - - - - -
    - - - - - - - K
    */

    EXPECT_TRUE(pawn_passed(&pos, B5));
    EXPECT_TRUE(pawn_passed(&pos, C4));
    EXPECT_TRUE(pawn_passed(&pos, D4));
    EXPECT_TRUE(pawn_passed(&pos, E5));

    EXPECT_FALSE(pawn_passed(&pos, F5));
    EXPECT_FALSE(pawn_passed(&pos, F4));
    EXPECT_FALSE(pawn_passed(&pos, G4));
    EXPECT_FALSE(pawn_passed(&pos, H5));
    EXPECT_FALSE(pawn_passed(&pos, H6));
}

TEST(eval_test, pawn_passed_wiki2)
{
    position_t pos;

    ASSERT_TRUE(
        set_pos(&pos,"8/5ppp/8/5PPP/8/6k1/8/6K1 w - - 0 1"));

    /*
    - - - - - - - -
    - - - - - p p p
    - - - - - - - -    white to move
    - - - - - P P P    no ep
    - - - - - - - -    no castling rights
    - - - - - - k -
    - - - - - - - -
    - - - - - - K -
    */

    EXPECT_FALSE(pawn_passed(&pos, F7));
    EXPECT_FALSE(pawn_passed(&pos, G7));
    EXPECT_FALSE(pawn_passed(&pos, H7));
    EXPECT_FALSE(pawn_passed(&pos, F5));
    EXPECT_FALSE(pawn_passed(&pos, G5));
    EXPECT_FALSE(pawn_passed(&pos, H5));
}

TEST(eval_test, pawn_passed_wiki3)
{
    position_t pos;

    ASSERT_TRUE(
        set_pos(&pos,"8/8/1PP2PbP/3r4/8/1Q5p/p5N1/k3K3 b - - 0 1"));

    /*
    - - - - - - - -
    - - - - - - - -
    - P P - - P b P    black to move
    - - - r - - - -    no ep
    - - - - - - - -    no castling rights
    - Q - - - - - p
    p - - - - - N -
    k - - - K - - -
    */

    EXPECT_TRUE(pawn_passed(&pos, B6));
    EXPECT_TRUE(pawn_passed(&pos, C6));
    EXPECT_TRUE(pawn_passed(&pos, F6));
    EXPECT_TRUE(pawn_passed(&pos, H6));
    EXPECT_TRUE(pawn_passed(&pos, A2));
    EXPECT_TRUE(pawn_passed(&pos, H3));
}

TEST(eval_test, pawn_passed_wiki4)
{
    position_t pos;

    ASSERT_TRUE(
        set_pos(&pos,"k7/b1P5/KP6/6q1/8/8/8/4n3 b - - 0 1"));

    /*
    k - - - - - - -
    b - P - - - - -
    K P - - - - - -    black to move
    - - - - - - q -    no ep
    - - - - - - - -    no castling rights
    - - - - - - - -
    - - - - - - - -
    - - - - n - - -
    */

    EXPECT_TRUE(pawn_passed(&pos, B6));
    EXPECT_TRUE(pawn_passed(&pos, C7));
}
