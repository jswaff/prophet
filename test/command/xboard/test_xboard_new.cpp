#include <prophet/error_codes.h>

#include <gtest/gtest.h>
#include <string.h>

#include "../../../src/command/xboard/xboard_internal.h"


extern position_t gpos;
extern bool xboard_force_mode;
extern int32_t max_depth;

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

    // and there is a depth constraint
    max_depth = 10;

    ASSERT_EQ(0, xboard_new("new"));

    // the global position should now be reset.
    position_t pos;
    memset(&pos, 0, sizeof(pos));
    reset_pos(&pos);

    EXPECT_EQ(0, memcmp(&gpos, &pos, sizeof(position_t)));

    // we should not be in force mode
    EXPECT_FALSE(xboard_force_mode);

    // depth constraints should be removed
    EXPECT_EQ(0, max_depth);
}
