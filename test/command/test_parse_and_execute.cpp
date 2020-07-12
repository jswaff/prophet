#include <prophet/error_codes.h>

#include <gtest/gtest.h>

#include "../../src/command/command_internal.h"

extern bool xboard_force_mode;

TEST(command_test, DISABLED_parse_and_execute_exit)
{
    bool exit_status;
    EXPECT_EQ(0, parse_and_execute("exit", &exit_status));
    EXPECT_TRUE(exit_status);
}

TEST(command_test, DISABLED_parse_and_execute_quit)
{
    bool exit_status;
    EXPECT_EQ(0, parse_and_execute("quit", &exit_status));
    EXPECT_TRUE(exit_status);
}

TEST(command_test, parse_and_execute_invalid_command)
{
    bool exit_status;
    EXPECT_EQ(0, parse_and_execute("bla bla", &exit_status));
    EXPECT_FALSE(exit_status);
}

TEST(command_test, parse_and_execute_xboard_force)
{
    bool exit_status;
    xboard_force_mode = false;
    EXPECT_EQ(0, parse_and_execute("force", &exit_status));
    EXPECT_TRUE(xboard_force_mode);
    EXPECT_FALSE(exit_status);
}
