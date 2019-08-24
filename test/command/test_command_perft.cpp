#include <prophet/error_codes.h>
#include <prophet/movegen.h>

#include <gtest/gtest.h>

#include "../../src/command/command_internal.h"

extern position gpos;

TEST(command_test, command_perft_incorrect_cmd)
{
    int exit_status;
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, 
        command_perft("bla", &exit_status));
}

TEST(command_test, command_perft_missing_n)
{
    int exit_status;
    EXPECT_EQ(P4_ERROR_CMD_PERFT_DEPTH,
        command_perft("perft", &exit_status));
}

TEST(command_test, command_perft)
{
    int exit_status;

    reset_pos(&gpos);
    EXPECT_EQ(0, command_perft("perft 2", &exit_status));

    // the exit status should be "no".
    EXPECT_EQ(0, exit_status);
}
