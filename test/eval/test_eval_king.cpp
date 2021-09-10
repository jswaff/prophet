#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

TEST(eval_test, eval_king)
{
    position_t pos;

    // as odd as this position is, it has all material on the board.
    // therefore, there should be no scaling of king safety.
    ASSERT_TRUE(set_pos(&pos, 
        "rnbq1rk1/pppppppp/bn6/8/BN6/5P2/PPPPP1PP/RNBQ1RK1 w - - 0 1"));

    int32_t wk = eval_king(&pos, G1, false);
    EXPECT_EQ(
        king_pst[G1] + eval_king_safety(&pos, true), 
        wk);


    EXPECT_EQ(
        king_pst[G1] + eval_king_safety(&pos, false),
        eval_king(&pos, G8, false));
}


TEST(eval_test, eval_king_endgame)
{
    // this position has no non-pawn material, so king safety isn't considered.
    position_t pos;
    ASSERT_TRUE(set_pos(&pos,"8/p3k3/8/8/8/8/4K3/8 w - - 0 1"));

    EXPECT_EQ(
        king_endgame_pst[E2], eval_king(&pos, E2, true));

    // test the symmetry
    EXPECT_EQ(eval_king(&pos, E2, true), eval_king(&pos, E7, true));
}
