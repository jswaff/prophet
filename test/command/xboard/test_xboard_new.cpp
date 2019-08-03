#include <gtest/gtest.h>

#include <prophet/position/position.h>

#include "../../../src/command/xboard/xboard_internal.h"

extern position gpos;

TEST(xboard_test, xboard_new)
{
    // make sure the global position is not already in the initial position.
    set_pos(&gpos, "8/8/3bk3/8/8/2K3Q1/8/8 w - - 0 1");

    int exit_status;

    EXPECT_EQ(0, xboard_new("new", &exit_status));

    // the global position should now be reset.
    position pos;
    reset_pos(&pos);

    EXPECT_EQ(0, memcmp(&gpos, &pos, sizeof(position)));

    // the exit status should be "no".
    EXPECT_EQ(0, exit_status);
}
