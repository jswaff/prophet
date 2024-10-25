#include <prophet/error_codes.h>
#include <prophet/movegen.h>

#include <gtest/gtest.h>

#include "../../src/command/command.h"

extern position_t gpos;

TEST(command_test, command_perft_incorrect_cmd)
{
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, command_perft("bla"));
}

TEST(command_test, command_perft_missing_n)
{
    EXPECT_EQ(P4_ERROR_CMD_PERFT_DEPTH, command_perft("perft"));
}

TEST(command_test, command_perft)
{
    reset_pos(&gpos);
    EXPECT_EQ(0, command_perft("perft 2"));
}
