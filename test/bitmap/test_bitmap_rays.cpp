#include <prophet/position/square.h>

#include <gtest/gtest.h>

#include "../src/bitmap/bitmap.h"

TEST(bitmap_test, rays)
{
	EXPECT_EQ(squares_to_bitmap(D6, D7, D8, NO_SQUARE), ray(D5, NORTH));
	EXPECT_EQ(squares_to_bitmap(C5, B5, A5, NO_SQUARE), ray(D5, WEST));
	EXPECT_EQ(squares_to_bitmap(E5, F5, G5, H5, NO_SQUARE), ray(D5, EAST));

	EXPECT_EQ(squares_to_bitmap(E6, F7, G8, NO_SQUARE), ray(D5, NORTHEAST));
	EXPECT_EQ(squares_to_bitmap(E4, F3, G2, H1, NO_SQUARE), ray(D5, SOUTHEAST));
	EXPECT_EQ(squares_to_bitmap(C4, B3, A2, NO_SQUARE), ray(D5, SOUTHWEST));
	EXPECT_EQ(squares_to_bitmap(C6, B7, A8, NO_SQUARE), ray(D5, NORTHWEST));
}
