#include <gtest/gtest.h>

#include <prophet/error_codes.h>

#include "../../../src/command/xboard/xboard_internal.h"

TEST(xboard_test, xboard_protover_incorrect_cmd)
{
    int exit_status;
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, 
        xboard_protover("bla", &exit_status));
}

TEST(xboard_test, xboard_protover_missing_n)
{
    int exit_status;
    EXPECT_EQ(P4_ERROR_CMD_XBOARD_PROTOVER_MISSING_N,
        xboard_protover("protover", &exit_status));
}

TEST(xboard_test, xboard_protover_unsupported_version)
{
    int exit_status;
    EXPECT_EQ(P4_ERROR_CMD_XBOARD_PROTOVER_UNSUPPORTED,
        xboard_protover("protover 1", &exit_status));
}

TEST(xboard_test, xboard_protover)
{
    int exit_status;
    EXPECT_EQ(0, xboard_protover("protover 2", &exit_status));
    EXPECT_EQ(0, exit_status);
}
