#include "command/xboard/xboard_internal.h"

#include "prophet/error_codes.h"

#include <gtest/gtest.h>


TEST(xboard_test, command_exit_incorrect_cmd)
{
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_quit("bla"));
}

TEST(xboard_test, command_exit)
{
    EXPECT_EQ(0, xboard_quit("exit"));
}

TEST(xboard_test, command_exit_with_quit)
{
    EXPECT_EQ(0, xboard_quit("quit"));
}
