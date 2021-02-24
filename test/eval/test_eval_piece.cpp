#include <prophet/const.h>
#include <prophet/eval.h>

#include <gtest/gtest.h>


TEST(eval_test, eval_piece)
{
	EXPECT_EQ(0, eval_piece(NO_PIECE));
	
	EXPECT_EQ(pawn_val, eval_piece(PAWN));
	EXPECT_EQ(pawn_val, eval_piece(-PAWN));

	EXPECT_EQ(knight_val, eval_piece(KNIGHT));
	EXPECT_EQ(knight_val, eval_piece(-KNIGHT));

	EXPECT_EQ(bishop_val, eval_piece(BISHOP));
	EXPECT_EQ(bishop_val, eval_piece(-BISHOP));

	EXPECT_EQ(rook_val, eval_piece(ROOK));
	EXPECT_EQ(rook_val, eval_piece(-ROOK));

	EXPECT_EQ(queen_val, eval_piece(QUEEN));
	EXPECT_EQ(queen_val, eval_piece(-QUEEN));

	EXPECT_EQ(INF, eval_piece(KING));
	EXPECT_EQ(INF, eval_piece(-KING));
}