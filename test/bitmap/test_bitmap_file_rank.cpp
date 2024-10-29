#include "../../src/bitmap/bitmap.h"

#include "prophet/square.h"

#include <gtest/gtest.h>


TEST(bitmap_test, sq_to_file_bitmap)
{
    EXPECT_EQ(sq_to_file_bitmap(A8), 
        square_to_bitmap(A8) | square_to_bitmap(A7) | 
        square_to_bitmap(A6) | square_to_bitmap(A5) |
        square_to_bitmap(A4) | square_to_bitmap(A3) |
        square_to_bitmap(A2) | square_to_bitmap(A1));

    EXPECT_EQ(sq_to_file_bitmap(F4), 
        square_to_bitmap(F8) | square_to_bitmap(F7) | 
        square_to_bitmap(F6) | square_to_bitmap(F5) |
        square_to_bitmap(F4) | square_to_bitmap(F3) |
        square_to_bitmap(F2) | square_to_bitmap(F1));

    EXPECT_EQ(sq_to_file_bitmap(H1), 
        square_to_bitmap(H8) | square_to_bitmap(H7) | 
        square_to_bitmap(H6) | square_to_bitmap(H5) |
        square_to_bitmap(H4) | square_to_bitmap(H3) |
        square_to_bitmap(H2) | square_to_bitmap(H1));
}


TEST(bitmap_test, sq_to_rank_bitmap)
{
    EXPECT_EQ(sq_to_rank_bitmap(A8), 
        square_to_bitmap(A8) | square_to_bitmap(B8) | 
        square_to_bitmap(C8) | square_to_bitmap(D8) |
        square_to_bitmap(E8) | square_to_bitmap(F8) |
        square_to_bitmap(G8) | square_to_bitmap(H8));

    EXPECT_EQ(sq_to_rank_bitmap(D2), 
        square_to_bitmap(A2) | square_to_bitmap(B2) | 
        square_to_bitmap(C2) | square_to_bitmap(D2) |
        square_to_bitmap(E2) | square_to_bitmap(F2) |
        square_to_bitmap(G2) | square_to_bitmap(H2));
}
