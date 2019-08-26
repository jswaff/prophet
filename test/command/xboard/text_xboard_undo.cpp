#include <prophet/error_codes.h>

#include <gtest/gtest.h>

#include "../../../src/command/xboard/xboard_internal.h"

extern position_t gpos;

TEST(xboard_test, xboard_undo_incorrect_cmd)
{
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_undo("bla"));
}

TEST(xboard_test, xboard_undo)
{
    // initialize the position and make two moves
    EXPECT_EQ(0, xboard_new("new"));
    EXPECT_EQ(0, xboard_force("force"));
    EXPECT_EQ(0, xboard_usermove("usermove e2e4"));
    EXPECT_EQ(0, xboard_usermove("usermove e7e5"));

    EXPECT_EQ(0, xboard_undo("undo"));

    // the last move should have been undone.
    position_t pos;
    reset_pos(&pos);
    undo_t u;
    apply_move(&pos, to_move(PAWN, E2, E4), &u);
    EXPECT_EQ(0, memcmp(&gpos, &pos, sizeof(position_t)));

    // another undo would be the initial position
    EXPECT_EQ(0, xboard_undo("undo"));

    // attempting another undo would be an error
    EXPECT_EQ(P4_ERROR_CMD_XBOARD_UNDO_INDEX_LOWER_BOUND,
        xboard_undo("undo"));

}

