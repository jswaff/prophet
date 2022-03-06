#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

TEST(eval_test, eval_rook)
{
    position_t pos;
    reset_pos(&pos);

    EXPECT_EQ(
        rook_pst[A1], eval_rook(&pos, A1, false));

    /* test the symmetry */
    EXPECT_EQ(eval_rook(&pos, A1, false), eval_rook(&pos, A8, false));
}


TEST(eval_test, eval_rook_endgame)
{
    position_t pos;
    reset_pos(&pos);

    EXPECT_EQ(
        rook_endgame_pst[A1], eval_rook(&pos, A1, true));

    /* test the symmetry */
    EXPECT_EQ(eval_rook(&pos, A1, true), eval_rook(&pos, A8, true));
}

TEST(eval_test, eval_rook_7th_king_back_rank)
{
    position_t pos;
    ASSERT_TRUE(set_pos(&pos, "7k/2Q2R2/8/8/8/8/r7/7K w - - 0 1"));
    
    EXPECT_EQ(rook_pst[F7] + major_on_7th + rook_open_file,
        eval_rook(&pos, F7, false));
}


TEST(eval_test, eval_rook_open_file)
{
    position_t pos;
    ASSERT_TRUE(set_pos(&pos, "3r3k/8/8/8/8/8/8/7K b - - 0 1"));

    EXPECT_EQ(rook_pst[D1] + rook_open_file,
        eval_rook(&pos, D8, false));
}

TEST(eval_test, eval_rook_half_open_file)
{
    position_t pos;

    /* friendly pawn but no enemy -- not half open (or open) */
    ASSERT_TRUE(set_pos(&pos, "8/2P5/8/2R5/K7/8/7k/8 w - - 0 1"));

    EXPECT_EQ(rook_pst[C5], eval_rook(&pos, C5, false));

    /* enemy pawn on C makes it half open */
    ASSERT_TRUE(set_pos(&pos, "8/2p5/8/2R5/K7/8/7k/8 w - - 0 1"));

    EXPECT_EQ(rook_pst[C5] + rook_half_open_file, 
        eval_rook(&pos, C5, false));
}
