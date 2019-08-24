#include <prophet/error_codes.h>

#include <gtest/gtest.h>

#include "../../../src/command/xboard/xboard_internal.h"

extern bool xboard_force_mode;

TEST(xboard_test, xboard_go_incorrect_cmd)
{
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_go("bla"));
}

TEST(xboard_test, xboard_go)
{
    xboard_force_mode = true;

    EXPECT_EQ(0, xboard_go("go"));

    EXPECT_FALSE(xboard_force_mode);
}
