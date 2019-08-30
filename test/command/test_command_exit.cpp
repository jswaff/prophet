#include <prophet/error_codes.h>

#include <gtest/gtest.h>

#include "../../src/command/command_internal.h"

TEST(command_test, command_exit_incorrect_cmd)
{
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, command_exit("bla"));
}

TEST(command_test, command_exit)
{
    EXPECT_EQ(0, command_exit("exit"));
}

TEST(command_test, command_exit_with_quit)
{
    EXPECT_EQ(0, command_exit("quit"));
}
