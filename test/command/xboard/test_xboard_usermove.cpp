#include <prophet/error_codes.h>

#include <gtest/gtest.h>

#include "../../../src/command/xboard/xboard_internal.h"

extern position_t gpos;
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
    EXPECT_EQ(0, xboard_new("new"));
    EXPECT_EQ(0, xboard_force("force"));
    EXPECT_EQ(P4_ERROR_CMD_XBOARD_USERMOVE_INVALID_MOVE, 
        xboard_usermove("usermove e3e4"));
}

TEST(xboard_test, xboard_usermove_illegal_move)
{
    EXPECT_EQ(0, xboard_new("new"));
    EXPECT_EQ(0, xboard_force("force"));
    EXPECT_EQ(P4_ERROR_CMD_XBOARD_USERMOVE_INVALID_MOVE, 
        xboard_usermove("usermove a7a5"));
}

TEST(xoard_test, xboard_usermove_force_mode)
{
    EXPECT_EQ(0, xboard_new("new"));
    EXPECT_EQ(0, xboard_force("force"));
    EXPECT_EQ(0, xboard_usermove("usermove e2e4"));

    // white should have moved, but not black (the engine)
    EXPECT_EQ(BLACK, gpos.player);
}

TEST(xoard_test, xboard_usermove_not_force_mode)
{
    ASSERT_EQ(0, xboard_new("new"));
    ASSERT_FALSE(xboard_force_mode);
    EXPECT_EQ(0, xboard_usermove("usermove e2e4"));

    // white should have moved, and then the engine.
    // Note: we may need to stop the search or limit the search depth in 
    // future versions.

    EXPECT_EQ(WHITE, gpos.player);
    EXPECT_EQ(2U, gpos.move_counter);
}

TEST(xboard_test, xboard_usermove_move_ends_game)
{
    ASSERT_EQ(0, xboard_new("new"));
    ASSERT_EQ(0, xboard_force("force"));
    ASSERT_EQ(0, xboard_usermove("usermove f2f3"));
    ASSERT_EQ(0, xboard_usermove("usermove e7e5"));
    ASSERT_EQ(0, xboard_usermove("usermove g2g4"));

    xboard_force_mode = false;

    EXPECT_EQ(0, xboard_usermove("usermove d8h4"));

    // TODO: assert "0-1 {Black mates}\n" was printed

    // even though we're note in force mode, it should still be white's move,
    // although white has move move - the game is over.
    EXPECT_EQ(WHITE, gpos.player); 
}
