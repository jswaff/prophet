#include "bitmap/bitmap.h"

#include <gtest/gtest.h>

TEST(bitmap_test, get_lsb)
{
    EXPECT_EQ(get_lsb(1), 0U);
    EXPECT_EQ(get_lsb(2), 1U);
    EXPECT_EQ(get_lsb(4), 2U);
    EXPECT_EQ(get_lsb(0xFFFFFFFFFFFFFFFF), 0U);
    EXPECT_EQ(get_lsb(0xF000000000000000), 60U);
    EXPECT_EQ(get_lsb(0x7000000000000000), 60U);
    EXPECT_EQ(get_lsb(0x0FFFFFFFFFFFFFFF), 0U);
}

