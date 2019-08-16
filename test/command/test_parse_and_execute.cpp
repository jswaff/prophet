#include <gtest/gtest.h>

#include <prophet/error_codes.h>

#include "../../src/command/command_internal.h"

extern bool xboard_force;

TEST(command_test, parse_and_execute_exit)
{
    int exit_status;

    EXPECT_EQ(0, parse_and_execute("exit", &exit_status));
    EXPECT_EQ(1, exit_status);
}

TEST(command_test, parse_and_execute_invalid_command)
{
    int exit_status;

    EXPECT_EQ(0, parse_and_execute("bla bla", &exit_status));
    EXPECT_EQ(0, exit_status);
}

TEST(command_test, parse_and_execute_xboard_force)
{
    int exit_status;

    xboard_force = false;

    EXPECT_EQ(0, parse_and_execute("force", &exit_status));
    EXPECT_EQ(0, exit_status);
//    EXPECT_TRUE(xboard_force);
}
