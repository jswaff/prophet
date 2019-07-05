#include <gtest/gtest.h>

#include <prophet/util/string_utils.h>

TEST(string_utils_test, str_to_sq)
{
    EXPECT_EQ(str_to_sq("-"), NO_SQUARE);
    EXPECT_EQ(str_to_sq("e3"), E3);
    EXPECT_EQ(str_to_sq("a8"), A8);
    EXPECT_EQ(str_to_sq("C5"), C5);
}

TEST(string_utils_test, sq_to_str)
{
    char* buf = sq_to_str(E3);
    EXPECT_EQ(strcmp(buf, "e3"), 0);
    free(buf);

    buf = sq_to_str(A8);
    EXPECT_EQ(strcmp(buf, "a8"), 0);
    free(buf);

    buf = sq_to_str(C5);
    EXPECT_EQ(strcmp(buf, "c5"), 0);
    free(buf);

    buf = sq_to_str(NO_SQUARE);
    EXPECT_EQ(buf, nullptr);
}