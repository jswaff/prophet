#include <prophet/error_codes.h>

#include <gtest/gtest.h>

#include "../../../src/command/xboard/xboard_internal.h"

TEST(xboard_test, xboard_protover_incorrect_cmd)
{
    EXPECT_EQ(P4_ERROR_CMD_INCORRECT_COMMAND, xboard_protover("bla"));
}

TEST(xboard_test, xboard_protover_missing_n)
{
    EXPECT_EQ(P4_ERROR_CMD_XBOARD_PROTOVER_MISSING_N, 
        xboard_protover("protover"));
}

/*TEST(xboard_test, xboard_protover_unsupported_version)
{
    EXPECT_EQ(P4_ERROR_CMD_XBOARD_PROTOVER_UNSUPPORTED, 
        xboard_protover("protover 1"));
}*/

TEST(xboard_test, xboard_protover)
{
    EXPECT_EQ(0, xboard_protover("protover 2"));
}
