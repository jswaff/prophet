#include <gtest/gtest.h>

#include <prophet/error_codes.h>

#include "../../../src/position/position_internal.h"
#include "../../../src/command/xboard/xboard_internal.h"


extern position gpos;
extern bool force_mode;

TEST(xboard_test, xboard_new_incorrect_cmd)
{
    int exit_status;
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, 
        xboard_new("bla", &exit_status));
}

TEST(xboard_test, xboard_new)
{
    // make sure the global position is not already in the initial position.
    ASSERT_TRUE(set_pos(&gpos, "8/8/3bk3/8/8/2K3Q1/8/8 w - - 0 1"));
    ASSERT_TRUE(verify_pos(&gpos));

    // and that we are in force mode
    force_mode = true;

    int exit_status;

    EXPECT_EQ(0, xboard_new("new", &exit_status));

    // the global position should now be reset.
    position pos;
    reset_pos(&pos);
    ASSERT_TRUE(verify_pos(&pos));

    EXPECT_EQ(0, memcmp(&gpos, &pos, sizeof(position)));

    // we should not be in force mode
    EXPECT_FALSE(force_mode);

    // the exit status should be "no".
    EXPECT_EQ(0, exit_status);
}
