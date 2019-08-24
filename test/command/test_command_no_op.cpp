#include <prophet/error_codes.h>

#include <gtest/gtest.h>

#include "../../src/command/command_internal.h"

TEST(command_test, command_no_op)
{
    int exit_status;

    EXPECT_EQ(0, command_no_op("bla", &exit_status));

    // the exit status should be "no".
    EXPECT_EQ(0, exit_status);
}
