#include "command/xboard/xboard_internal.h"

#include "prophet/error_codes.h"

#include <gtest/gtest.h>

#include <stdint.h>
#include <string.h>

extern bool fixed_time_per_move;
extern uint32_t time_control_moves;
extern char time_control_base[10];
extern double time_control_increment;


TEST(xboard_test, xboard_level_incorrect_cmd)
{
    EXPECT_EQ(ERROR_CMD_INCORRECT_COMMAND, xboard_level("bla"));
}

TEST(xboard_test, xboard_level_missing_parameter)
{
    EXPECT_EQ(ERROR_CMD_XBOARD_LEVEL_MISSING_PARAM, xboard_level("level 0    2"));
}


TEST(xboard_test, xboard_level)
{
    fixed_time_per_move = true;
    time_control_moves = 0;
    time_control_base[0] = 0;
    time_control_increment = 0;

    ASSERT_EQ(0, xboard_level("level 0 2 12"));
    EXPECT_EQ(0U, time_control_moves);
    EXPECT_EQ(0, strcmp("2", time_control_base));
    EXPECT_TRUE(time_control_increment > 11.9999F);
    EXPECT_TRUE(time_control_increment < 12.0001F);
    EXPECT_FALSE(fixed_time_per_move);

    // another variation
    ASSERT_EQ(0, xboard_level("level  40    0:30  0 "));
    EXPECT_EQ(40U, time_control_moves);
    EXPECT_EQ(0, strcmp("0:30", time_control_base));
    EXPECT_TRUE(time_control_increment == 0.0F);

    // another variation
    ASSERT_EQ(0, xboard_level("level 0 10 0.5"));
    EXPECT_EQ(0U, time_control_moves);
    printf("time control base: %s\n", time_control_base);
    EXPECT_EQ(0, strcmp("10", time_control_base));
    EXPECT_TRUE(time_control_increment > 0.49999F);
    EXPECT_TRUE(time_control_increment < 0.50001F);
}
