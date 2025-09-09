#include "command/xboard/xboard_internal.h"

#include "prophet/error_codes.h"
#include "prophet/position.h"

#include <gtest/gtest.h>

#include <stdint.h>
#include <string.h>

extern position_t gpos;
extern int32_t max_depth;
extern bool xboard_force_mode;
extern bool xboard_post_mode;

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
    xboard_post_mode = false;
    max_depth = 1;

    // redirect stdout to a buffer 
    char buffer[255];
    memset(buffer, 0, 255);
    ASSERT_EQ(stdout, freopen("/dev/null", "a", stdout));
    setbuf(stdout, buffer);

    int retval = xboard_usermove("usermove e2e4");
    ASSERT_EQ(0, block_on_search_thread(false)); // wait for search thread to finish

    // redirect back
    ASSERT_EQ(stdout, freopen("/dev/tty", "a", stdout));

    ASSERT_EQ(0, retval);

    // white should have moved, and then the engine.
    // ensure a move was printed
    EXPECT_EQ(0, strncmp(buffer, "move ", 5));

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

    // redirect stdout to a buffer 
    char buffer[255];
    memset(buffer, 0, 255);
    ASSERT_EQ(stdout, freopen("/dev/null", "a", stdout));
    setbuf(stdout, buffer);

    int retval = xboard_usermove("usermove d8h4");

    // redirect back
    ASSERT_EQ(stdout, freopen("/dev/tty", "a", stdout));

    ASSERT_EQ(0, retval);
    EXPECT_EQ(0, strcmp("0-1 {Black mates}\n", buffer));

    // even though we're note in force mode, it should still be white's move,
    // although white has move move - the game is over.
    EXPECT_EQ(WHITE, gpos.player); 
}
