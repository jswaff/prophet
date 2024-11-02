#include "command/xboard/xboard_internal.h"

#include "prophet/error_codes.h"

#include <gtest/gtest.h>


extern bool xboard_force_mode;

TEST(xboard_test, xboard_force_incorrect_cmd)
{
    ASSERT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_force("bla"));
}

TEST(xboard_test, xboard_force)
{
    xboard_force_mode = false;

    ASSERT_EQ(0, xboard_force("force"));

    ASSERT_TRUE(xboard_force_mode);
}
