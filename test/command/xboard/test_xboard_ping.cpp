#include <prophet/error_codes.h>

#include <gtest/gtest.h>

#include "../../../src/command/xboard/xboard_internal.h"

TEST(xboard_test, xboard_ping_incorrect_cmd)
{
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_ping("bla"));
}

TEST(xboard_test, xboard_ping_missing_n)
{
    EXPECT_EQ(P4_ERROR_CMD_XBOARD_PING_MISSING_N, xboard_ping("ping"));
}

TEST(xboard_test, xboard_ping)
{
    EXPECT_EQ(0, xboard_ping("ping 1"));
}
