#include <prophet/error_codes.h>

#include <gtest/gtest.h>

#include "../../../src/command/xboard/xboard_internal.h"

extern position_t gpos;
extern bool xboard_force_mode;

TEST(xboard_test, xboard_go_incorrect_cmd)
{
    ASSERT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_go("bla"));
}

TEST(xboard_test, xboard_go)
{
	// setup
    ASSERT_EQ(0, xboard_new("new"));
    ASSERT_EQ(0, xboard_force("force"));

	// redirect stdout to a buffer 
	char buffer[255];
	memset(buffer, 0, 255);
	freopen("/dev/null", "a", stdout);
	setbuf(stdout, buffer);

    int retval = xboard_go("go");

    // redirect back
    freopen("/dev/tty", "a", stdout);

    ASSERT_EQ(0, retval);

    // Note: in future versions we'll have to stop the search, or maye just
    // set a shallow search depth

    // ensure a move was printed
    EXPECT_EQ(0, strncmp(buffer, "move ", 5));

    EXPECT_FALSE(xboard_force_mode);

    // white should have made a move
    EXPECT_EQ(BLACK, gpos.player);

}
