#include <gtest/gtest.h>

#include <prophet/position/position.h>
#include <prophet/error_codes.h>

#include "../../../src/command/xboard/xboard_internal.h"

extern position gpos;
extern bool xboard_force_mode;

TEST(xboard_test, xboard_usermove_incorrect_cmd)
{
    int exit_status;
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, 
        xboard_usermove("bla", &exit_status));
}

TEST(xboard_test, xboard_usermove_missing_move)
{
    int exit_status;

    EXPECT_EQ(P4_ERROR_CMD_XBOARD_USERMOVE_MISSING_MOVE, 
        xboard_usermove("usermove", &exit_status));

}

TEST(xboard_test, xboard_usermove_invalid_move)
{
    int exit_status;

    EXPECT_EQ(P4_ERROR_CMD_XBOARD_USERMOVE_INVALID_MOVE, 
        xboard_usermove("usermove bla", &exit_status));

}

TEST(xboard_test, xboard_usermove_illegal_move)
{
    int exit_status;

    reset_pos(&gpos);

    EXPECT_EQ(P4_ERROR_CMD_XBOARD_USERMOVE_INVALID_MOVE, 
        xboard_usermove("usermove e3e4", &exit_status));
}

TEST(xoard_test, xboard_usermove)
{
    int exit_status;

    reset_pos(&gpos);

    EXPECT_EQ(0, xboard_usermove("usermove e2e4", &exit_status));

    EXPECT_EQ(0, exit_status);
}