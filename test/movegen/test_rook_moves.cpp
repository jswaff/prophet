#include <gtest/gtest.h>

#include "../../src/movegen/movegen_internal.h"


TEST(movegen_test, rook_moves_initial_pos)
{
    position pos;
    reset_pos(&pos);

    move_t moves[10];
    move_t* mp = gen_rook_moves(moves, &pos, true, true);

    EXPECT_EQ(mp - moves, 0);
}

TEST(movegen_test, rook_moves)
{
    position pos;
    set_pos(&pos,"8/8/3k1p2/8/3K4/8/1R3r2/8 b - - 0 1");

    move_t moves[20];
    move_t* mp = gen_rook_moves(moves, &pos, true, true);

    EXPECT_EQ(mp - moves, 10);

    EXPECT_TRUE(move_list_contains(to_move(ROOK, F2, F3), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(ROOK, F2, F4), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(ROOK, F2, F5), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(ROOK, F2, F1), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(ROOK, F2, G2), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(ROOK, F2, H2), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(ROOK, F2, E2), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(ROOK, F2, D2), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(ROOK, F2, C2), moves, mp));
    EXPECT_TRUE(move_list_contains(to_capture(ROOK, F2, B2, ROOK), moves, mp));
}

TEST(movegen_test, rook_captures)
{
    position pos;
    set_pos(&pos,"8/8/3k1p2/8/3K4/8/1R3r2/8 b - - 0 1");

    move_t moves[10];
    move_t* mp = gen_rook_moves(moves, &pos, true, false);

    EXPECT_EQ(mp - moves, 1);

    EXPECT_TRUE(move_list_contains(to_capture(ROOK, F2, B2, ROOK), moves, mp));
}

TEST(movegen_test, rook_noncaptures)
{
    position pos;
    set_pos(&pos,"8/8/3k1p2/8/3K4/8/1R3r2/8 b - - 0 1");

    move_t moves[20];
    move_t* mp = gen_rook_moves(moves, &pos, false, true);

    EXPECT_EQ(mp - moves, 9);

    EXPECT_TRUE(move_list_contains(to_move(ROOK, F2, F3), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(ROOK, F2, F4), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(ROOK, F2, F5), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(ROOK, F2, F1), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(ROOK, F2, G2), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(ROOK, F2, H2), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(ROOK, F2, E2), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(ROOK, F2, D2), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(ROOK, F2, C2), moves, mp));
}
