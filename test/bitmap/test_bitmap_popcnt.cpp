#include <gtest/gtest.h>

#include "../../src/bitmap/bitmap.h"

TEST(bitmap_test, popcnt)
{
    EXPECT_EQ(popcnt(0), 0U);
    EXPECT_EQ(popcnt(3), 2U);
    EXPECT_EQ(popcnt(5), 2U);
    EXPECT_EQ(popcnt(15), 4U);
    EXPECT_EQ(popcnt(0xFF), 8U);
    EXPECT_EQ(popcnt(0xF0F0), 8U);
    EXPECT_EQ(popcnt(0x7777), 12U);
    EXPECT_EQ(popcnt(0xFFFFFFFF), 32U);
    EXPECT_EQ(popcnt(0xFFFFFFFFFFFFFFFF), 64U);
}
