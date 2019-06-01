#include <gtest/gtest.h>

#include <prophet/util/string_utils.h>

TEST(string_utils_test, str_to_sq)
{
    EXPECT_EQ(str_to_sq("-"), NO_SQUARE);
    EXPECT_EQ(str_to_sq("e3"), E3);
    EXPECT_EQ(str_to_sq("a8"), A8);
    EXPECT_EQ(str_to_sq("C5"), C5);
}
