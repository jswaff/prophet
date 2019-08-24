#include <prophet/error_codes.h>

#include <gtest/gtest.h>

#include "../../src/command/command_internal.h"

TEST(command_test, command_exit_incorrect_cmd)
{
    int exit_status;
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, 
        command_exit("bla", &exit_status));
}

TEST(command_test, command_exit)
{
    int exit_status;

    EXPECT_EQ(0, command_exit("exit", &exit_status));

    // the exit status should be "yes".
    EXPECT_EQ(1, exit_status);
}

TEST(command_test, command_exit_with_quit)
{
    int exit_status;

    EXPECT_EQ(0, command_exit("quit", &exit_status));

    // the exit status should be "yes".
    EXPECT_EQ(1, exit_status);
}
