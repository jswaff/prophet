#include <prophet/error_codes.h>

#include <gtest/gtest.h>

#include "../../src/command/command_internal.h"

extern bool xboard_force_mode;

TEST(command_test, parse_and_execute_exit)
{
    EXPECT_EQ(0, parse_and_execute("exit"));
}

TEST(command_test, parse_and_execute_invalid_command)
{
    EXPECT_EQ(0, parse_and_execute("bla bla"));
}

TEST(command_test, parse_and_execute_xboard_force)
{
    xboard_force_mode = false;
    EXPECT_EQ(0, parse_and_execute("force"));
    EXPECT_TRUE(xboard_force_mode);
}
