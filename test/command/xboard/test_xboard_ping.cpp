#include "command/xboard/xboard_internal.h"

#include "prophet/error_codes.h"

#include <gtest/gtest.h>

#include <string.h>


TEST(xboard_test, xboard_ping_incorrect_cmd)
{
    ASSERT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_ping("bla"));
}

TEST(xboard_test, xboard_ping_missing_n)
{
    ASSERT_EQ(P4_ERROR_CMD_XBOARD_PING_MISSING_N, xboard_ping("ping"));
}

TEST(xboard_test, xboard_ping)
{
	// redirect stdout to a buffer 
	char buffer[255];
	memset(buffer, 0, 255);
	ASSERT_EQ(stdout, freopen("/dev/null", "a", stdout));
	setbuf(stdout, buffer);

	int retval = xboard_ping("ping 1337");

    // redirect back
    ASSERT_EQ(stdout, freopen("/dev/tty", "a", stdout));

    ASSERT_EQ(0, retval);

    ASSERT_EQ(0, strcmp("pong 1337\n", buffer));
}
