#include <gtest/gtest.h>

#include <prophet/position/piece.h>

#include "../../src/position/position_internal.h"

TEST(pos_test, add_piece)
{
    position pos;
    reset_pos(&pos);

    add_piece(&pos, PAWN, E4);
    EXPECT_EQ(pos.piece[E4], PAWN);
    EXPECT_EQ(
        pos.white_pawns, 
        squares_to_bitmap(A2, B2, C2, D2, E2, F2, G2, H2, E4, NO_SQUARE));

    add_piece(&pos, -PAWN, E5);
    EXPECT_EQ(pos.piece[E5], -PAWN);
    EXPECT_EQ(
        pos.black_pawns, 
        squares_to_bitmap(A7, B7, C7, D7, E7, F7, G7, H7, E5, NO_SQUARE));


    add_piece(&pos, KNIGHT, F3);
    EXPECT_EQ(pos.piece[F3], KNIGHT);
    EXPECT_EQ(pos.white_knights, squares_to_bitmap(B1, G1, F3, NO_SQUARE));
}
