#include "command/xboard/xboard_internal.h"

#include "prophet/error_codes.h"

#include <gtest/gtest.h>


extern bool xboard_post_mode;

TEST(xboard_test, xboard_post_incorrect_cmd)
{
    ASSERT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_post("bla"));
}

TEST(xboard_test, xboard_post)
{
    xboard_post_mode = false;

    ASSERT_EQ(0, xboard_post("post"));

    ASSERT_TRUE(xboard_post_mode);
}
