#include <prophet/error_codes.h>

#include <gtest/gtest.h>

#include "../../src/command/command.h"

TEST(command_test, command_no_op)
{
    EXPECT_EQ(0, command_no_op("bla"));
}
