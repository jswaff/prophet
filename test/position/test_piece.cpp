#include <gtest/gtest.h>

#include <prophet/position/piece.h>

TEST(piece_test, is_white_piece)
{
    EXPECT_TRUE(is_white_piece(PAWN));
    EXPECT_FALSE(is_white_piece(-PAWN));
    EXPECT_FALSE(is_white_piece(NO_PIECE));
}

TEST(piece_test, is_black_piece)
{
    EXPECT_TRUE(is_black_piece(-QUEEN));
    EXPECT_FALSE(is_black_piece(KNIGHT));
    EXPECT_FALSE(is_black_piece(NO_PIECE));
}

TEST(piece_test, is_not_white_piece)
{
    EXPECT_TRUE(is_not_white_piece(NO_PIECE));
    EXPECT_TRUE(is_not_white_piece(-KING));
    EXPECT_FALSE(is_not_white_piece(BISHOP));
}

TEST(piece_test, is_not_black_piece)
{
    EXPECT_TRUE(is_not_black_piece(NO_PIECE));
    EXPECT_TRUE(is_not_black_piece(ROOK));
    EXPECT_FALSE(is_not_black_piece(-KNIGHT));
}
