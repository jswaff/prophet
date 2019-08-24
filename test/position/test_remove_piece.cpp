#include <gtest/gtest.h>

#include <prophet/position/piece.h>

#include "../../src/position/position_internal.h"


TEST(pos_test, remove_piece)
{
    position pos;
    reset_pos(&pos);

    remove_piece(&pos,E2);
    EXPECT_EQ(pos.piece[E2], NO_PIECE);
    EXPECT_EQ(pos.white_pawns, 
        squares_to_bitmap(A2, B2, C2, D2, F2, G2, H2, NO_SQUARE));

    remove_piece(&pos,H1);
    EXPECT_EQ(pos.piece[H1], NO_PIECE);
    EXPECT_EQ(pos.white_rooks, squares_to_bitmap(A1, NO_SQUARE));

    remove_piece(&pos,F8);
    EXPECT_EQ(pos.piece[F8], NO_PIECE);
    EXPECT_EQ(pos.black_bishops, squares_to_bitmap(C8, NO_SQUARE));
}
