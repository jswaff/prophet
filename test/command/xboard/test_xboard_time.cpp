#include <prophet/error_codes.h>

#include <gtest/gtest.h>
#include <string.h>

#include "../../../src/command/xboard/xboard_internal.h"

extern uint32_t time_remaining_millis;


TEST(xboard_test, xboard_time_incorrect_cmd)
{
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_time("bla"));
}

TEST(xboard_test, xboard_time)
{
    // reset the time remaining
    time_remaining_millis = 0;

    ASSERT_EQ(0, xboard_time("time 1000")); // 10 seconds in centiseconds

    EXPECT_EQ(10000U, time_remaining_millis);
}
