#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

TEST(eval_test, eval_scale_large_val)
{
	EXPECT_EQ(100, eval_scale(100, all_nonpawn_pieces_val));

	// we expect the term to be scaled down progressivly as material is 
	// removed from the board.
	EXPECT_EQ(89, eval_scale(100, 
		queen_val + rook_val*2 + bishop_val + knight_val*2));

	EXPECT_EQ(80, eval_scale(100, 
		queen_val + rook_val*2 + bishop_val + knight_val));

	EXPECT_EQ(44, eval_scale(100, queen_val + rook_val));

	EXPECT_EQ(15, eval_scale(100, rook_val));

	EXPECT_EQ(9, eval_scale(100, knight_val));
}


TEST(eval_test, eval_scale_small_val)
{
	EXPECT_EQ(5, eval_scale(5, all_nonpawn_pieces_val));

	// we expect the term to be scaled down progressivly as material is 
	// removed from the board.
	EXPECT_EQ(4, eval_scale(5, 
		queen_val + rook_val*2 + bishop_val + knight_val*2));

	EXPECT_EQ(4, eval_scale(5, 
		queen_val + rook_val*2 + bishop_val + knight_val));

	EXPECT_EQ(2, eval_scale(5, queen_val + rook_val));

	EXPECT_EQ(0, eval_scale(5, rook_val));

	EXPECT_EQ(0, eval_scale(5, knight_val));
}


TEST(eval_test, eval_scale_negative_val)
{
	EXPECT_EQ(-33, eval_scale(-33, all_nonpawn_pieces_val));

	// we expect the term to be scaled down progressivly as material is 
	// removed from the board.
	EXPECT_EQ(-29, eval_scale(-33, 
		queen_val + rook_val*2 + bishop_val + knight_val*2));

	EXPECT_EQ(-26, eval_scale(-33, 
		queen_val + rook_val*2 + bishop_val + knight_val));

	EXPECT_EQ(-14, eval_scale(-33, queen_val + rook_val));

	EXPECT_EQ(-5, eval_scale(-33, rook_val));

	EXPECT_EQ(-3, eval_scale(-33, knight_val));
}
