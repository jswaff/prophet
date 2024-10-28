#include "prophet/position/position.h"

#include "../../src/position/position_internal.h"

#include <gtest/gtest.h>

#include <string.h>

TEST(pos_test, pos_flipped)
{
    position_t pos;
    reset_pos(&pos);

    position_t flipped = position_flip(&pos);
    position_t reflipped = position_flip(&flipped);

    EXPECT_FALSE(pos_equals(&pos, &flipped, true));
    EXPECT_TRUE(pos_equals(&pos, &reflipped, true));
    EXPECT_EQ(0, memcmp(&pos, &reflipped, sizeof(position_t)));

    /* a more complicated position */
    ASSERT_TRUE(set_pos(&pos,"7r/R6p/2K4P/5k1P/2p4n/5p2/8/8 w - - 0 1"));
    flipped = position_flip(&pos);
    reflipped = position_flip(&flipped);
    EXPECT_EQ(0, memcmp(&pos, &reflipped, sizeof(position_t)));

    /* slightly more complicated, with one side castling rights and ep */
    ASSERT_TRUE(set_pos(&pos, 
        "r2r2k1/1p2p1b1/1pp1b1pp/4P2n/PP3p1p/2P2N2/5PP1/R3KBNR b KQ b3 - 0 1"));
    flipped = position_flip(&pos);
    reflipped = position_flip(&flipped);
    EXPECT_EQ(0, memcmp(&pos, &reflipped, sizeof(position_t)));
}
