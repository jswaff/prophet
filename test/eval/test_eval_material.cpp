#include "eval/eval_internal.h"

#include "prophet/position.h"

#include "position/position_internal.h"

#include <gtest/gtest.h>

TEST(eval_test, eval_nonpawn_material)
{
    position_t pos;
    ASSERT_TRUE(set_pos(&pos, "rq5k/8/8/8/8/8/8/BNK5 w - - 0 1"));

    EXPECT_EQ(
        bishop_val + knight_val + (knight_kaufman_adj * -5),
        eval_nonpawn_material(&pos, true));

    EXPECT_EQ(
        rook_val + (rook_kaufman_adj * -5) + queen_val,
        eval_nonpawn_material(&pos, false));
}


TEST(eval_test, eval_pawn_material_initial_pos)
{
    position_t pos;
    reset_pos(&pos);

    EXPECT_EQ(
        pawn_val * 8,
        eval_pawn_material(&pos, true));

    EXPECT_EQ(
        pawn_val * 8,
        eval_pawn_material(&pos, false));
}


TEST(eval_test, eval_pawn_material)
{
    position_t pos;
    ASSERT_TRUE(set_pos(&pos, "rq5k/p7/8/8/8/8/8/BNK5 w - - 0 1"));

    EXPECT_EQ(0, eval_pawn_material(&pos, true));

    EXPECT_EQ(pawn_val, eval_pawn_material(&pos, false));
}
