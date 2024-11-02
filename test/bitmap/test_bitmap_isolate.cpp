#include "bitmap/bitmap.h"

#include <gtest/gtest.h>

#include <stdint.h>

TEST(bitmap_test, isolate_bit)
{
    uint64_t mask = 26; // 11010

    EXPECT_EQ(isolate_bit(mask, 0), (uint64_t)2);
    EXPECT_EQ(isolate_bit(mask, 1), (uint64_t)8);
    EXPECT_EQ(isolate_bit(mask, 2), (uint64_t)16);
    EXPECT_EQ(isolate_bit(mask, 3), (uint64_t)0);
}
