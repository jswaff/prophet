#include <prophet/error_codes.h>

#include <gtest/gtest.h>

#include "../../../src/command/xboard/xboard_internal.h"

TEST(xboard_test, xboard_result_incorrect_cmd)
{
    int exit_status;
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, 
        xboard_result("bla", &exit_status));
}

TEST(xboard_test, xboard_result_missing_result)
{
    int exit_status;
    EXPECT_EQ(P4_ERROR_CMD_XBOARD_RESULT_MISSING_RESULT,
        xboard_result("result", &exit_status));
}

TEST(xboard_test, xboard_result_invalid_result)
{
    int exit_status;

    EXPECT_EQ(0, xboard_result("result 1-0", &exit_status));

    EXPECT_EQ(0, xboard_result("result 0-1", &exit_status));

    EXPECT_EQ(0, xboard_result("result 1/2-1/2", &exit_status));

    EXPECT_EQ(0, xboard_result("result *", &exit_status));

    EXPECT_EQ(0, xboard_result("result 1/2-1/2 {with some comment}", 
        &exit_status));

    EXPECT_EQ(P4_ERROR_CMD_XBOARD_RESULT_INVALID_RESULT, 
        xboard_result("result bla", &exit_status));
}
