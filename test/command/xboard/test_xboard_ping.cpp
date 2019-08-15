#include <gtest/gtest.h>

#include <prophet/error_codes.h>

#include "../../../src/command/xboard/xboard_internal.h"

TEST(xboard_test, xboard_ping_incorrect_cmd)
{
    int exit_status;
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, 
        xboard_ping("bla", &exit_status));
}

TEST(xboard_test, xboard_ping_missing_n)
{
    int exit_status;
    EXPECT_EQ(P4_ERROR_CMD_XBOARD_PING_MISSING_N,
        xboard_ping("ping", &exit_status));
}

TEST(xboard_test, xboard_ping)
{
    int exit_status;
    EXPECT_EQ(0, xboard_ping("ping 1", &exit_status));
    EXPECT_EQ(0, exit_status);
}
