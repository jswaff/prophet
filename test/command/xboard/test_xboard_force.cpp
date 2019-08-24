#include <prophet/error_codes.h>

#include <gtest/gtest.h>

#include "../../../src/command/xboard/xboard_internal.h"

extern bool xboard_force_mode;

TEST(xboard_test, xboard_force_incorrect_cmd)
{
    int exit_status;
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, 
        xboard_force("bla", &exit_status));
}

TEST(xboard_test, xboard_force)
{
    int exit_status;

    xboard_force_mode = false;

    EXPECT_EQ(0, xboard_force("force", &exit_status));

    EXPECT_TRUE(xboard_force_mode);

    // the exit status should be "no".
    EXPECT_EQ(0, exit_status);
}
