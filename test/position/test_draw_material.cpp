#include "prophet/position.h"

#include <gtest/gtest.h>


TEST(pos_test, draw_material)
{
    position_t pos;

    // no material
    set_pos(&pos, "kb6/8/1K6/8/8/8/8/8 b - - ");
    EXPECT_TRUE(is_lack_of_mating_material(&pos));

    // one pawn
    set_pos(&pos, "4k3/8/8/8/8/8/P7/4K3 w - -");
    EXPECT_FALSE(is_lack_of_mating_material(&pos));

    // one knight
    set_pos(&pos, "4k3/8/8/8/8/8/n7/4K3 w - -");
    EXPECT_TRUE(is_lack_of_mating_material(&pos));

    // one bishop
    set_pos(&pos, "4k3/8/8/8/8/8/B7/4K3 w - -");
    EXPECT_TRUE(is_lack_of_mating_material(&pos));

    // one rook
    set_pos(&pos, "4k3/8/8/8/8/8/r7/4K3 b - -");
    EXPECT_FALSE(is_lack_of_mating_material(&pos));

    // one queen
    set_pos(&pos, "4k3/8/8/8/8/8/Q7/4K3 w - -");
    EXPECT_FALSE(is_lack_of_mating_material(&pos));

    // two white knights
    set_pos(&pos, "4k3/8/8/8/8/8/NN6/4K3 w - -");
    EXPECT_FALSE(is_lack_of_mating_material(&pos));

    // two opposing knights
    set_pos(&pos, "4k3/8/8/8/8/8/Nn6/4K3 b - -");
    EXPECT_FALSE(is_lack_of_mating_material(&pos));

    // two opposing bishops different colors
    set_pos(&pos, "4k3/8/8/8/8/8/Bb6/4K3 w - -");
    EXPECT_FALSE(is_lack_of_mating_material(&pos));

    // two opposing bishops same color
    set_pos(&pos, "4k3/8/8/8/8/8/B1b5/4K3 b - -");
    EXPECT_TRUE(is_lack_of_mating_material(&pos));

    // bishop vs knight
    set_pos(&pos, "4k3/8/8/8/8/8/B1n5/5K2 b - -");
    EXPECT_FALSE(is_lack_of_mating_material(&pos));
}
