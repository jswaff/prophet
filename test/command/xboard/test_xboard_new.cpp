#include <prophet/error_codes.h>

#include <gtest/gtest.h>
#include <string.h>

#include "../../../src/command/xboard/xboard_internal.h"


extern position gpos;
extern bool xboard_force_mode;

TEST(xboard_test, xboard_new_incorrect_cmd)
{
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_new("bla"));
}

TEST(xboard_test, xboard_new)
{
    // make sure the global position is not already in the initial position.
    memset(&gpos, 0, sizeof(gpos));

    // and that we are in force mode
    xboard_force_mode = true;

    ASSERT_EQ(0, xboard_new("new"));

    // the global position should now be reset.
    position pos;
    memset(&pos, 0, sizeof(pos));
    reset_pos(&pos);

    EXPECT_EQ(0, memcmp(&gpos, &pos, sizeof(position)));

    // we should not be in force mode
    EXPECT_FALSE(xboard_force_mode);
}
