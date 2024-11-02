#include "command/xboard/xboard_internal.h"

#include "prophet/error_codes.h"

#include <gtest/gtest.h>


extern bool xboard_post_mode;

TEST(xboard_test, xboard_nopost_incorrect_cmd)
{
    ASSERT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_nopost("bla"));
}

TEST(xboard_test, xboard_nopost)
{
    xboard_post_mode = true;

    ASSERT_EQ(0, xboard_nopost("nopost"));

    ASSERT_FALSE(xboard_post_mode);
}
