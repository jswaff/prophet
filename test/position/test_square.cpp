#include "position/square_internal.h"

#include "prophet/square.h"
#include "prophet/position.h"

#include "bitmap/bitmap.h"

#include <gtest/gtest.h>

#include <stdint.h>

TEST(bitmap_test, squares_to_bitmap)
{
    EXPECT_EQ(squares_to_bitmap(NO_SQUARE), (uint64_t)0x00);
    EXPECT_EQ(squares_to_bitmap(A8, NO_SQUARE), (uint64_t)0x01);
    EXPECT_EQ(squares_to_bitmap(B8, NO_SQUARE), (uint64_t)0x02);
    // 2 + 4 + 8
    EXPECT_EQ(squares_to_bitmap(B8, C8, D8, NO_SQUARE), (uint64_t)0x0E);  
}

TEST(square_test, file_to_bitmap)
{
    EXPECT_EQ(file_to_bitmap(FILE_A), 
        squares_to_bitmap(A1, A2, A3, A4, A5, A6, A7, A8, NO_SQUARE));
    EXPECT_EQ(file_to_bitmap(FILE_B), 
        squares_to_bitmap(B1, B2, B3, B4, B5, B6, B7, B8, NO_SQUARE));
    EXPECT_EQ(file_to_bitmap(FILE_C), 
        squares_to_bitmap(C1, C2, C3, C4, C5, C6, C7, C8, NO_SQUARE));
    EXPECT_EQ(file_to_bitmap(FILE_D), 
        squares_to_bitmap(D1, D2, D3, D4, D5, D6, D7, D8, NO_SQUARE));
    EXPECT_EQ(file_to_bitmap(FILE_E), 
        squares_to_bitmap(E1, E2, E3, E4, E5, E6, E7, E8, NO_SQUARE));
    EXPECT_EQ(file_to_bitmap(FILE_F), 
        squares_to_bitmap(F1, F2, F3, F4, F5, F6, F7, F8, NO_SQUARE));
    EXPECT_EQ(file_to_bitmap(FILE_G), 
        squares_to_bitmap(G1, G2, G3, G4, G5, G6, G7, G8, NO_SQUARE));
    EXPECT_EQ(file_to_bitmap(FILE_H), 
        squares_to_bitmap(H1, H2, H3, H4, H5, H6, H7, H8, NO_SQUARE));
}

TEST(square_test, rank_to_bitmap)
{
    EXPECT_EQ(rank_to_bitmap(RANK_1), 
        squares_to_bitmap(A1, B1, C1, D1, E1, F1, G1, H1, NO_SQUARE));
    EXPECT_EQ(rank_to_bitmap(RANK_2), 
        squares_to_bitmap(A2, B2, C2, D2, E2, F2, G2, H2, NO_SQUARE));
    EXPECT_EQ(rank_to_bitmap(RANK_3), 
        squares_to_bitmap(A3, B3, C3, D3, E3, F3, G3, H3, NO_SQUARE));
    EXPECT_EQ(rank_to_bitmap(RANK_4), 
        squares_to_bitmap(A4, B4, C4, D4, E4, F4, G4, H4, NO_SQUARE));
    EXPECT_EQ(rank_to_bitmap(RANK_5), 
        squares_to_bitmap(A5, B5, C5, D5, E5, F5, G5, H5, NO_SQUARE));
    EXPECT_EQ(rank_to_bitmap(RANK_6), 
        squares_to_bitmap(A6, B6, C6, D6, E6, F6, G6, H6, NO_SQUARE));
    EXPECT_EQ(rank_to_bitmap(RANK_7), 
        squares_to_bitmap(A7, B7, C7, D7, E7, F7, G7, H7, NO_SQUARE));
    EXPECT_EQ(rank_to_bitmap(RANK_8), 
        squares_to_bitmap(A8, B8, C8, D8, E8, F8, G8, H8, NO_SQUARE));
}

TEST(square_test, get_rank)
{
    EXPECT_EQ(get_rank(A8), RANK_8);
    EXPECT_EQ(get_rank(B8), RANK_8);
    EXPECT_EQ(get_rank(H8), RANK_8);
    EXPECT_EQ(get_rank(C7), RANK_7);
    EXPECT_EQ(get_rank(E5), RANK_5);
    EXPECT_EQ(get_rank(A2), RANK_2);
    EXPECT_EQ(get_rank(H1), RANK_1);
}

TEST(square_test, get_file)
{
    EXPECT_EQ(get_file(A1), FILE_A);
    EXPECT_EQ(get_file(B3), FILE_B);
    EXPECT_EQ(get_file(C8), FILE_C);
    EXPECT_EQ(get_file(D4), FILE_D);
    EXPECT_EQ(get_file(E2), FILE_E);
    EXPECT_EQ(get_file(F7), FILE_F);
    EXPECT_EQ(get_file(G5), FILE_G);
    EXPECT_EQ(get_file(H6), FILE_H);
}

TEST(square_test, get_dir)
{
    EXPECT_EQ(get_dir(E4, E6), NORTH);
    EXPECT_EQ(get_dir(D6, D1), SOUTH);
    EXPECT_EQ(get_dir(H1, A1), WEST);
    EXPECT_EQ(get_dir(A7, G7), EAST);

    EXPECT_EQ(get_dir(B3, E6), NORTHEAST);
    EXPECT_EQ(get_dir(A5, E1), SOUTHEAST);
    EXPECT_EQ(get_dir(H3, G2), SOUTHWEST);
    EXPECT_EQ(get_dir(G5, E7), NORTHWEST);

    EXPECT_EQ(get_dir(C3, D5), NODIR);
    EXPECT_EQ(get_dir(E4, E4), NODIR);
}

TEST(square_test, file_distance)
{
    EXPECT_EQ(file_distance(A1,B1), 1U);
    EXPECT_EQ(file_distance(A1,H8), 7U);
    EXPECT_EQ(file_distance(A1,H4), 7U);
    EXPECT_EQ(file_distance(B3,C7), 1U);
    EXPECT_EQ(file_distance(H2,A3), 7U);
    EXPECT_EQ(file_distance(F3,B8), 4U);
    EXPECT_EQ(file_distance(E1,E2), 0U);
}

TEST(square_test, rank_distance)
{
    EXPECT_EQ(rank_distance(A1,B1), 0U);
    EXPECT_EQ(rank_distance(A1,C4), 3U);
    EXPECT_EQ(rank_distance(H8,A7), 1U);
    EXPECT_EQ(rank_distance(G8,B1), 7U);
}

TEST(square_test, distance)
{
    EXPECT_EQ(distance(E4,E4), 0U);
    EXPECT_EQ(distance(A1,A8), 7U);
    EXPECT_EQ(distance(H1,E4), 3U);
    EXPECT_EQ(distance(H8,E4), 4U);
}

TEST(square_test, north)
{
    EXPECT_EQ(north(NO_SQUARE), NO_SQUARE);
    EXPECT_EQ(north(B4), B5);
    EXPECT_EQ(north(B8), NO_SQUARE);
}

TEST(square_test, east)
{
    EXPECT_EQ(east(NO_SQUARE), NO_SQUARE);
    EXPECT_EQ(east(B4), C4);
    EXPECT_EQ(east(H4), NO_SQUARE);
}

TEST(square_test, south)
{
    EXPECT_EQ(south(NO_SQUARE), NO_SQUARE);
    EXPECT_EQ(south(B4), B3);
    EXPECT_EQ(south(B1), NO_SQUARE);
}

TEST(square_test, west)
{
    EXPECT_EQ(west(NO_SQUARE), NO_SQUARE);
    EXPECT_EQ(west(B4), A4);
    EXPECT_EQ(west(A4), NO_SQUARE);
}

TEST(square_test, northeast)
{
    EXPECT_EQ(northeast(NO_SQUARE), NO_SQUARE);
    EXPECT_EQ(northeast(B4), C5);
    EXPECT_EQ(northeast(G8), NO_SQUARE);
    EXPECT_EQ(northeast(H7), NO_SQUARE);
}

TEST(square_test, southeast)
{
    EXPECT_EQ(southeast(NO_SQUARE), NO_SQUARE);
    EXPECT_EQ(southeast(B4), C3);
    EXPECT_EQ(southeast(G1), NO_SQUARE);
    EXPECT_EQ(southeast(H2), NO_SQUARE);
}

TEST(square_test, southwest)
{
    EXPECT_EQ(southwest(NO_SQUARE), NO_SQUARE);
    EXPECT_EQ(southwest(B4), A3);
    EXPECT_EQ(southwest(B1), NO_SQUARE);
    EXPECT_EQ(southwest(A2), NO_SQUARE);
}

TEST(square_test, northwest)
{
    EXPECT_EQ(northwest(NO_SQUARE), NO_SQUARE);
    EXPECT_EQ(northwest(B4), A5);
    EXPECT_EQ(northwest(A7), NO_SQUARE);
    EXPECT_EQ(northwest(B8), NO_SQUARE);
}

TEST(square_test, is_light_sq)
{
    EXPECT_TRUE(is_light_sq(A8));
    EXPECT_FALSE(is_light_sq(B8));
    EXPECT_TRUE(is_light_sq(C8));
    EXPECT_FALSE(is_light_sq(A7));
    EXPECT_TRUE(is_light_sq(B7));
    EXPECT_FALSE(is_light_sq(C7));
    EXPECT_TRUE(is_light_sq(A6));
    EXPECT_FALSE(is_light_sq(B6));
    EXPECT_TRUE(is_light_sq(C6));
}

TEST(square_test, is_dark_sq)
{
    EXPECT_TRUE(is_dark_sq(H8));
    EXPECT_FALSE(is_dark_sq(G8));
    EXPECT_TRUE(is_dark_sq(F8));
    EXPECT_FALSE(is_dark_sq(H7));
    EXPECT_TRUE(is_dark_sq(G7));
    EXPECT_FALSE(is_dark_sq(F7));
    EXPECT_TRUE(is_dark_sq(H6));
    EXPECT_FALSE(is_dark_sq(G6));
    EXPECT_TRUE(is_dark_sq(F6));
}

TEST(square_test, is_diagonal)
{
    EXPECT_TRUE(is_diagonal(A1,B2));
    EXPECT_FALSE(is_diagonal(A1,A2));
    EXPECT_TRUE(is_diagonal(D4,G7));
    EXPECT_TRUE(is_diagonal(G7,B2));
    EXPECT_FALSE(is_diagonal(C2,E2));
    EXPECT_TRUE(is_diagonal(A8,H1));
}
