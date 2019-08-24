#include <prophet/error_codes.h>

#include <gtest/gtest.h>

#include "../../../src/command/xboard/xboard_internal.h"

extern position gpos;
extern bool xboard_force_mode;

TEST(xboard_test, xboard_usermove_incorrect_cmd)
{
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_usermove("bla"));
}

TEST(xboard_test, xboard_usermove_missing_move)
{
    EXPECT_EQ(P4_ERROR_CMD_XBOARD_USERMOVE_MISSING_MOVE, 
        xboard_usermove("usermove"));

}

TEST(xboard_test, xboard_usermove_junk_move)
{
    EXPECT_EQ(P4_ERROR_CMD_XBOARD_USERMOVE_INVALID_MOVE, 
        xboard_usermove("usermove bla"));

}

TEST(xboard_test, xboard_usermove_invalid_move)
{
    reset_pos(&gpos);

    EXPECT_EQ(P4_ERROR_CMD_XBOARD_USERMOVE_INVALID_MOVE, 
        xboard_usermove("usermove e3e4"));
}

TEST(xboard_test, xboard_usermove_illegal_move)
{
    reset_pos(&gpos);

    EXPECT_EQ(P4_ERROR_CMD_XBOARD_USERMOVE_INVALID_MOVE, 
        xboard_usermove("usermove a7a5"));
}

TEST(xoard_test, xboard_usermove)
{
    reset_pos(&gpos);

    EXPECT_EQ(0, xboard_usermove("usermove e2e4"));
}
