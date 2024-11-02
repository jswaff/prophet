#include "command/xboard/xboard_internal.h"

#include "prophet/error_codes.h"

#include <gtest/gtest.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern position_t gpos;
extern int32_t max_depth;
extern bool xboard_force_mode;
extern bool xboard_post_mode;

TEST(xboard_test, xboard_go_incorrect_cmd)
{
    ASSERT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_go("bla"));
}

TEST(xboard_test, xboard_go)
{
	// setup
    ASSERT_EQ(0, xboard_new("new"));
    ASSERT_EQ(0, xboard_force("force"));
    xboard_post_mode = false;
    max_depth = 1;

	// redirect stdout to a buffer 
	char buffer[255];
	memset(buffer, 0, 255);
	freopen("/dev/null", "a", stdout);
	setbuf(stdout, buffer);

    int retval = xboard_go("go");
    ASSERT_EQ(0, block_on_search_thread(false)); // wait for search thread to finish

    // redirect back
    freopen("/dev/tty", "a", stdout);

    ASSERT_EQ(0, retval);

    // ensure a move was printed
    EXPECT_EQ(0, strncmp(buffer, "move ", 5));

    EXPECT_FALSE(xboard_force_mode);

    // white should have made a move
    EXPECT_EQ(BLACK, gpos.player);

}
