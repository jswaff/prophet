#include <gtest/gtest.h>

#include "../../src/command/command_internal.h"

TEST(command_test, command_exit)
{
    int exit_status;

    EXPECT_EQ(0, command_exit("bla", &exit_status));

    // the exit status should be "yes".
    EXPECT_EQ(1, exit_status);
}
