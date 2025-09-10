#include "command/xboard/xboard_internal.h"

#include "prophet/error_codes.h"

#include "position/position_internal.h"

#include <gtest/gtest.h>

#include <string.h>

extern position_t gpos;

TEST(xboard_test, xboard_setboard_incorrect_cmd)
{
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_setboard("bla"));
}


TEST(xboard_test, xboard_setboard_missing_fen)
{
    EXPECT_EQ(
        P4_ERROR_CMD_XBOARD_USERMOVE_MISSING_FEN, 
        xboard_setboard("setboard bla"));
}


TEST(xboard_test, xboard_setboard_invalid_fen)
{
    reset_pos(&gpos);

    // no kings
    EXPECT_EQ(
        P4_ERROR_CMD_XBOARD_USERMOVE_INVALID_FEN,
        xboard_setboard("setboard 8/pppppppp/8/8/8/8/8/8 w KQkq 0 1"));

    // position should be initial position
    position_t pos;
    reset_pos(&pos);
    EXPECT_EQ(0, memcmp(&gpos, &pos, sizeof(position_t)));
}


TEST(xboard_test, xboard_setboard)
{
    ASSERT_EQ(0, 
        xboard_setboard(
            "setboard rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));

    position_t pos;
    reset_pos(&pos);

    ASSERT_EQ(0, memcmp(&gpos, &pos, sizeof(position_t)));
}
