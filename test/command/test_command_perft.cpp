#include "command/command.h"

#include "prophet/error_codes.h"

#include "position/position.h"

#include <gtest/gtest.h>

extern position_t gpos;

TEST(command_test, command_perft_incorrect_cmd)
{
    EXPECT_EQ(ERROR_CMD_INCORRECT_COMMAND, command_perft("bla"));
}

TEST(command_test, command_perft_missing_n)
{
    EXPECT_EQ(ERROR_CMD_PERFT_DEPTH, command_perft("perft"));
}

TEST(command_test, command_perft)
{
    reset_pos(&gpos);
    EXPECT_EQ(0, command_perft("perft 2"));
}
