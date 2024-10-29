#include "../../src/util/prng.h"

#include <gtest/gtest.h>

#include <stdint.h>

static int hamming_distance(uint64_t x, uint64_t y);

/**
 * Test that the test utility function to measure hamming distances is correct.
 */
TEST(prng_test, hamming_distance)
{
    EXPECT_EQ(hamming_distance(0, 0), 0);
    EXPECT_EQ(hamming_distance(0, 1), 1);
    EXPECT_EQ(hamming_distance(0, 3), 2);
    EXPECT_EQ(hamming_distance(7, 1), 2);

    uint64_t max32 = 0xFFFFFFFF;
    EXPECT_EQ(hamming_distance(0,max32), 32);

    uint64_t max64 = 0xFFFFFFFFFFFFFFFF;

    EXPECT_EQ(hamming_distance(0,max64), 64);
    EXPECT_EQ(hamming_distance(max64,0), 64);

    EXPECT_EQ(hamming_distance(max32,max64), 32);
}

/**
 * Test the 32 bit PRNG.
 *
 * Here we just generate a bunch of random numbers and ensure that, on average, half the bits are flipping.
 *
 * A proper test would use some sort of statistical analysis, like a chi-square test.
 */
TEST(prng_test, rand32)
{
    int total_hamming_distance = 0;

    for (int i=0;i<10000;i++) {
        total_hamming_distance += hamming_distance(random32(), random32());
    }

    float avg_hamming_distance = (float)total_hamming_distance / 10000;

    EXPECT_GT(avg_hamming_distance, 15.9);
    EXPECT_LT(avg_hamming_distance, 16.1);
}

TEST(prng_test, rand64)
{
    int total_hamming_distance = 0;

    for (int i=0;i<10000;i++) {
        total_hamming_distance += hamming_distance(random64(), random64());
    }

    float avg_hamming_distance = (float)total_hamming_distance / 10000;

    EXPECT_GT(avg_hamming_distance, 31.9);
    EXPECT_LT(avg_hamming_distance, 32.1);
}

/**
 * Utility function to measure the hamming distance between two 64-bit words.
 */
static int hamming_distance(uint64_t x, uint64_t y)
{
    int dist = 0;

    uint64_t val = x ^ y;

    while (val != (uint64_t)0) {
        ++dist;
        val &= val - 1;
    }

    return dist;
}
