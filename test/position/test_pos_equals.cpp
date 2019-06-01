#include <gtest/gtest.h>

#include <prophet/position/position.h>
#include <prophet/position/piece.h>

#include "../../src/position/position_internal.h"

TEST(pos_test, pos_equals)
{
    position pos1;
    reset_pos(&pos1);

    position pos2;
    reset_pos(&pos2);

    EXPECT_TRUE(pos_equals(&pos1, &pos2, true));

    add_piece(&pos2, PAWN, E4);
    EXPECT_FALSE(pos_equals(&pos1, &pos2, true));

    remove_piece(&pos2, E4);
    EXPECT_TRUE(pos_equals(&pos1, &pos2, true));
}
