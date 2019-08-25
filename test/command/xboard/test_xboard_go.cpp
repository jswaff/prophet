#include <prophet/error_codes.h>

#include <gtest/gtest.h>

#include "../../../src/command/xboard/xboard_internal.h"

extern position_t gpos;
extern bool xboard_force_mode;

TEST(xboard_test, xboard_go_incorrect_cmd)
{
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_go("bla"));
}

TEST(xboard_test, xboard_go)
{
    reset_pos(&gpos);

    xboard_force_mode = true;

    EXPECT_EQ(0, xboard_go("go"));

    // Note: in future versions we'll have to stop the search, or maye just
    // set a shallow search depth

    EXPECT_FALSE(xboard_force_mode);

    // white should have made a move
    EXPECT_EQ(BLACK, gpos.player);
}
