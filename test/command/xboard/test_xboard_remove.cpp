#include <prophet/error_codes.h>

#include <gtest/gtest.h>

#include "../../../src/command/xboard/xboard_internal.h"

extern position_t gpos;

TEST(xboard_test, xboard_remove_incorrect_cmd)
{
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_remove("bla"));
}

TEST(xboard_test, xboard_remove_test)
{
    // initialize the position and make three moves
    ASSERT_EQ(0, xboard_new("new"));
    ASSERT_EQ(0, xboard_force("force"));
    ASSERT_EQ(0, xboard_usermove("usermove e2e4"));
    ASSERT_EQ(0, xboard_usermove("usermove e7e5"));
    ASSERT_EQ(0, xboard_usermove("usermove d2d4"));

    ASSERT_EQ(0, xboard_remove("remove"));

    // the last two moves should have been undone.
    position_t pos;
    reset_pos(&pos);
    undo_t u;
    apply_move(&pos, to_move(PAWN, E2, E4), &u);

    EXPECT_EQ(0, memcmp(&gpos, &pos, sizeof(position_t)));

    // attempting another remove would be an error
    EXPECT_EQ(P4_ERROR_GUNDO_INDEX_LB_VIOLATION,
        xboard_remove("remove"));
}
