#include "eval/eval_internal.h"

#include "position/position_internal.h"

#include <gtest/gtest.h>

TEST(eval_test, pawn_supports)
{
    position_t pos;

    reset_pos(&pos);

    undo_t u;
    apply_move(&pos, to_move(PAWN, E2, E4), &u);
    EXPECT_FALSE(pawn_supports(&pos, E4));

    apply_move(&pos, to_move(KNIGHT, G8, F6), &u);
    EXPECT_TRUE(pawn_supports(&pos, F6));

    apply_move(&pos, to_move(PAWN, D2, D3), &u);
    EXPECT_TRUE(pawn_supports(&pos, E4));
    EXPECT_TRUE(pawn_supports(&pos, D3));

}
