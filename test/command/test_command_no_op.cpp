#include "../../src/command/command.h"

#include <gtest/gtest.h>

TEST(command_test, command_no_op)
{
    EXPECT_EQ(0, command_no_op("bla"));
}
