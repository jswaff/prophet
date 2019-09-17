#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

TEST(eval_test, eval_king)
{
    position_t pos;
    ASSERT_TRUE(set_pos(&pos, 
        "rnbq1rk1/pppppppp/8/8/8/5P2/PPPPP1PP/RNBQ1RK1 w - - 0 1"));

    int32_t wk = eval_king(&pos, G1, all_nonpawn_pieces_val);
    EXPECT_EQ(
        king_pst[G1] + eval_king_safety(&pos, true), 
        wk);


    EXPECT_EQ(
        king_pst[G1] + eval_king_safety(&pos, false),
        eval_king(&pos, G8, all_nonpawn_pieces_val));
}


TEST(eval_test, eval_king_endgame)
{
    position_t pos;
    ASSERT_TRUE(set_pos(&pos,"8/p3k3/8/8/8/8/4K3/8 w - - 0 1"));

    EXPECT_EQ(
        king_endgame_pst[E2], eval_king(&pos, E2, 0));

    /* test the symmetry */
    EXPECT_EQ(eval_king(&pos, E2, 0), eval_king(&pos, E7, 0));
}
