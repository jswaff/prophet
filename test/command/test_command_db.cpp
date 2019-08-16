#include <gtest/gtest.h>

#include <prophet/position/position.h>
#include <prophet/error_codes.h>

#include "../../src/command/command_internal.h"

extern position gpos;

TEST(command_test, command_db_incorrect_cmd)
{
    int exit_status;
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, 
        command_db("bla", &exit_status));
}

TEST(command_test, command_db)
{
    int exit_status;

    reset_pos(&gpos);
    EXPECT_EQ(0, command_db("db", &exit_status));

    // the exit status should be "no".
    EXPECT_EQ(0, exit_status);
}
