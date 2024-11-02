#include "prophet/position.h"

#include "eval/eval_internal.h"
#include "position/square_internal.h"

#include <gtest/gtest.h>

#include <stdint.h>

TEST(eval_test, eval_knight)
{
    position_t pos;
    reset_pos(&pos);

    int32_t mg = 0; int32_t eg = 0;
    eval_knight(&pos, B1, &mg, &eg);
    EXPECT_EQ(knight_pst_mg[B1] + knight_tropism_mg * (int32_t)distance(B1, E8), mg);
    EXPECT_EQ(knight_pst_eg[B1] + knight_tropism_eg * (int32_t)distance(B1, E8), eg);

    /* test the symmetry */
    int32_t mg2 = 0; int32_t eg2 = 0;
    eval_knight(&pos, B8, &mg2, &eg2);
    EXPECT_EQ(mg, -mg2);
    EXPECT_EQ(eg, -eg2);
}

TEST(eval_test, eval_knight_outpost)
{
    position_t pos;
    ASSERT_TRUE(set_pos(&pos, 
        "r1br1k2/pp3pp1/1b4np/4P3/2pNN3/2P3B1/PP1R1PPP/3R2K1 w - - 0 1"));

    int32_t mg = 0; int32_t eg = 0;
    eval_knight(&pos, D4, &mg, &eg);

    EXPECT_EQ(knight_pst_mg[D4] + 
        knight_tropism_mg * (int32_t)distance(D4, F8) +
        knight_outpost[D4] +
        knight_supported_outpost[D4], mg);

    EXPECT_EQ(knight_pst_eg[D4] + 
        knight_tropism_eg * (int32_t)distance(D4, F8) +
        knight_outpost[D4] +
        knight_supported_outpost[D4], eg);
}
