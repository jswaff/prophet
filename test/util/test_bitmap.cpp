#include <prophet/util/bitmap.h>

#include <gtest/gtest.h>


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

TEST(bitmap_test, get_msb)
{
    EXPECT_EQ(get_msb(1), 0U);
    EXPECT_EQ(get_msb(2), 1U);
    EXPECT_EQ(get_msb(4), 2U);
    EXPECT_EQ(get_msb(0xFFFFFFFFFFFFFFFF), 63U);
    EXPECT_EQ(get_msb(0xF000000000000000), 63U);
    EXPECT_EQ(get_msb(0x7000000000000000), 62U);
    EXPECT_EQ(get_msb(0x0FFFFFFFFFFFFFFF), 59U);
}

TEST(bitmap_test, isolate_bit)
{
    uint64_t mask = 26; // 11010

    EXPECT_EQ(isolate_bit(mask, 0), (uint64_t)2);
    EXPECT_EQ(isolate_bit(mask, 1), (uint64_t)8);
    EXPECT_EQ(isolate_bit(mask, 2), (uint64_t)16);
    EXPECT_EQ(isolate_bit(mask, 3), (uint64_t)0);
}
