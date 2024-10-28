#include "../../src/eval/eval_internal.h"

#include "prophet/position/position.h"

#include <gtest/gtest.h>

#include <stdint.h>

TEST(eval_test, eval_rook)
{
    position_t pos;
    reset_pos(&pos);

    int32_t mg = 0; int32_t eg = 0;
    eval_rook(&pos, A1, &mg, &eg);
    EXPECT_EQ(rook_pst_mg[A1] + rook_mobility_mg[0], mg);
    EXPECT_EQ(rook_pst_eg[A1] + rook_mobility_eg[0], eg);


    /* test the symmetry */
    int32_t mg2 = 0; int32_t eg2 = 0;
    eval_rook(&pos, A8, &mg2, &eg2);
    EXPECT_EQ(mg, -mg2);
    EXPECT_EQ(eg, -eg2);
}


TEST(eval_test, eval_rook_7th_king_back_rank)
{
    position_t pos;
    ASSERT_TRUE(set_pos(&pos, "7k/2Q2R2/8/8/8/8/r7/7K w - - 0 1"));

    int32_t mg = 0; int32_t eg = 0;
    eval_rook(&pos, F7, &mg, &eg);
    EXPECT_EQ(rook_pst_mg[F7] + major_on_7th_mg + connected_majors_on_7th_mg + rook_open_file_mg + 
        rook_mobility_mg[11], mg);
    EXPECT_EQ(rook_pst_eg[F7] + major_on_7th_eg + connected_majors_on_7th_eg + rook_open_file_eg + 
        rook_mobility_eg[11], eg);
}


TEST(eval_test, eval_rook_open_file)
{
    position_t pos;
    ASSERT_TRUE(set_pos(&pos, "3r3k/8/8/8/8/8/8/7K b - - 0 1"));

    int32_t mg = 0; int32_t eg = 0;
    eval_rook(&pos, D8, &mg, &eg);
    EXPECT_EQ(-(rook_pst_mg[D1] + rook_open_file_mg + rook_mobility_mg[13]), mg);
    EXPECT_EQ(-(rook_pst_eg[D1] + rook_open_file_eg + rook_mobility_eg[13]), eg);
}

TEST(eval_test, DISABLED_eval_rook_open_file_supported)
{
    position_t pos;
    ASSERT_TRUE(set_pos(&pos, "7k/8/8/8/8/3R4/8/3R3K w - - 0 1"));

    int32_t mg = 0; int32_t eg = 0;
    eval_rook(&pos, D1, &mg, &eg);
    EXPECT_EQ(rook_pst_mg[D1] + rook_open_file_mg + rook_open_file_supported_mg + rook_mobility_mg[7], mg);
    EXPECT_EQ(rook_pst_eg[D1] + rook_open_file_eg + rook_open_file_supported_eg + rook_mobility_eg[7], eg);
}

TEST(eval_test, eval_rook_half_open_file)
{
    position_t pos;

    /* friendly pawn but no enemy -- not half open (or open) */
    ASSERT_TRUE(set_pos(&pos, "8/2P5/8/2R5/K7/8/7k/8 w - - 0 1"));

    int32_t mg = 0; int32_t eg = 0;
    eval_rook(&pos, C5, &mg, &eg);
    EXPECT_EQ(rook_pst_mg[C5] + rook_mobility_mg[12], mg);
    EXPECT_EQ(rook_pst_eg[C5] + rook_mobility_eg[12], eg);

    /* enemy pawn on C makes it half open */
    ASSERT_TRUE(set_pos(&pos, "8/2p5/8/2R5/K7/8/7k/8 w - - 0 1"));

    int32_t mg2 = 0; int32_t eg2 = 0;
    eval_rook(&pos, C5, &mg2, &eg2);
    EXPECT_EQ(rook_pst_mg[C5] + rook_half_open_file_mg + rook_mobility_mg[12], mg2);
    EXPECT_EQ(rook_pst_eg[C5] + rook_half_open_file_eg + rook_mobility_eg[12], eg2);
}
