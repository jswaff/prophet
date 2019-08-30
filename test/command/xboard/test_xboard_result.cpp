#include <prophet/error_codes.h>

#include <gtest/gtest.h>

#include "../../../src/command/xboard/xboard_internal.h"

TEST(xboard_test, xboard_result_incorrect_cmd)
{
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_result("bla"));
}

TEST(xboard_test, xboard_result_missing_result)
{
    EXPECT_EQ(P4_ERROR_CMD_XBOARD_RESULT_MISSING_RESULT,
        xboard_result("result"));
}

TEST(xboard_test, xboard_result_invalid_result)
{
    EXPECT_EQ(0, xboard_result("result 1-0"));

    EXPECT_EQ(0, xboard_result("result 0-1"));

    EXPECT_EQ(0, xboard_result("result 1/2-1/2"));

    EXPECT_EQ(0, xboard_result("result *"));

    EXPECT_EQ(0, xboard_result("result 1/2-1/2 {with some comment}"));

    EXPECT_EQ(P4_ERROR_CMD_XBOARD_RESULT_INVALID_RESULT, 
        xboard_result("result bla"));
}
