#include "../../src/eval/eval_internal.h"

#include "prophet/position.h"

#include <gtest/gtest.h>

#include <stdint.h>

TEST(eval_test, eval_king)
{
    position_t pos;

    // as odd as this position is, it has all material on the board.
    // therefore, there should be no scaling of king safety.
    ASSERT_TRUE(set_pos(&pos, 
        "rnbq1rk1/pppppppp/bn6/8/BN6/5P2/PPPPP1PP/RNBQ1RK1 w - - 0 1"));

    int32_t mg = 0; int32_t eg = 0;
    eval_king(&pos, G1, &mg, &eg);
    EXPECT_EQ(king_pst_mg[G1] + eval_king_safety(&pos, true), mg);
    EXPECT_EQ(king_pst_eg[G1], eg);
}


TEST(eval_test, eval_king2)
{
    position_t pos;
    ASSERT_TRUE(set_pos(&pos,"8/p3k3/8/8/8/8/4K3/8 w - - 0 1"));

    int32_t mg = 0; int32_t eg = 0;
    eval_king(&pos, E2, &mg, &eg);
    EXPECT_EQ(king_pst_mg[E2] + eval_king_safety(&pos, true), mg);
    EXPECT_EQ(king_pst_eg[E2], eg);

    // test the symmetry
    int32_t mg2 = 0; int32_t eg2 = 0;
    eval_king(&pos, E7, &mg2, &eg2);
    EXPECT_EQ(mg, -mg2);
    EXPECT_EQ(eg, -eg2);
}
