#include <prophet/error_codes.h>

#include <gtest/gtest.h>
#include <string.h>

#include "../../../src/command/xboard/xboard_internal.h"

extern int32_t max_depth;

TEST(xboard_test, xboard_sd_incorrect_cmd)
{
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_sd("bla"));
}

TEST(xboard_test, xboard_sd)
{
    // reset the depth constraint
    max_depth = 0;

    ASSERT_EQ(0, xboard_sd("sd 3"));

    // the depth constraint should be set
    EXPECT_EQ(3, max_depth);
}
