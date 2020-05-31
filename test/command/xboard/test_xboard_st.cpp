#include <prophet/error_codes.h>

#include <gtest/gtest.h>
#include <string.h>

#include "../../../src/command/xboard/xboard_internal.h"

extern uint32_t max_time_ms;
extern bool fixed_time_per_move;


TEST(xboard_test, xboard_st_incorrect_cmd)
{
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_st("bla"));
}

TEST(xboard_test, xboard_st)
{
    // reset the time constraint
    max_time_ms = 0;
    fixed_time_per_move = false;

    ASSERT_EQ(0, xboard_st("st 10"));

    // the time constraint should be set
    EXPECT_EQ(10000U, max_time_ms);
    EXPECT_TRUE(fixed_time_per_move);
}
