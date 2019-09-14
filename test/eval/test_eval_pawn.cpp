#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

TEST(eval_test, eval_pawn)
{
    position_t pos;
    reset_pos(&pos);

    /* test the piece square term */
    EXPECT_EQ(pawn_pst[E2], eval_pawn(&pos, E2));

    /* test the symmetry */
    EXPECT_EQ(pawn_pst[E2], eval_pawn(&pos, E7));
}


TEST(eval_test, eval_pawn_wiki3)
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

    /* the white pawn on B6 is passed */
    EXPECT_EQ(pawn_pst[B6] + passed_pawn,
        eval_pawn(&pos, B6));

    /* the black pawn on A2 is passed and isolated */
    EXPECT_EQ(pawn_pst[A7] + passed_pawn + isolated_pawn,
        eval_pawn(&pos, A2));
}
