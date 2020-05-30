#include <prophet/error_codes.h>

#include <gtest/gtest.h>
#include <string.h>

#include "../../../src/command/xboard/xboard_internal.h"


extern uint32_t time_control_moves;
extern char time_control_base[10];
extern uint32_t time_control_increment;



TEST(xboard_test, xboard_level_incorrect_cmd)
{
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_level("bla"));
}

TEST(xboard_test, xboard_level_missing_parameter)
{
    EXPECT_EQ(P4_ERROR_CMD_XBOARD_LEVEL_MISSING_PARAM, xboard_level("level 0    2"));
}


TEST(xboard_test, xboard_level)
{
    time_control_moves = 0;
    time_control_base[0] = 0;
    time_control_increment = 0;

    // reset the moves, base time, and increment variables
    ASSERT_EQ(0, xboard_level("level 0 2 12"));
    EXPECT_EQ(0U, time_control_moves);
    EXPECT_EQ(0, strcmp("2", time_control_base));
    EXPECT_EQ(12U, time_control_increment);

    // another variation
    ASSERT_EQ(0, xboard_level("level  40    0:30  0 "));
    EXPECT_EQ(40U, time_control_moves);
    EXPECT_EQ(0, strcmp("0:30", time_control_base));
    EXPECT_EQ(0U, time_control_increment);
}
