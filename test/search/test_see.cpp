#include "search/search_internal.h"

#include "prophet/move.h"
#include "prophet/movegen.h"
#include "prophet/position.h"

#include "movegen/movegen_internal.h"

#include <gtest/gtest.h>

#include <stdint.h>

TEST(see_test, queen_takes_undefended_pawn)
{
    position_t pos;
    set_pos(&pos, "7k/8/1p6/8/8/1Q6/8/7K w - -");

    move_t moves[50], *endp;
    endp = gen_legal_moves(moves, &pos, true, true); 

    move_t b3b6 = to_capture(QUEEN, B3, B6, PAWN);
    ASSERT_TRUE(move_list_contains(b3b6, moves, endp));

    int32_t score = see(&pos, b3b6);
    ASSERT_EQ(score, see_pawn_val);
}


TEST(see_test, queen_takes_defended_pawn)
{
    position_t pos;
    set_pos(&pos, "7k/p7/1p6/8/8/1Q6/8/7K w - -");

    move_t moves[50], *endp;
    endp = gen_legal_moves(moves, &pos, true, true); 

    move_t b3b6 = to_capture(QUEEN, B3, B6, PAWN);
    ASSERT_TRUE(move_list_contains(b3b6, moves, endp));

    int32_t score = see(&pos, b3b6);
    ASSERT_EQ(score, see_pawn_val - see_queen_val);
}


TEST(see_test, rook_takes_undefended_pawn)
{
    position_t pos;
    set_pos(&pos, "1k1r4/1pp4p/p7/4p3/8/P5P1/1PP4P/2K1R3 w - -");

    move_t moves[50], *endp;
    endp = gen_legal_moves(moves, &pos, true, true); 

    move_t e1e5 = to_capture(ROOK, E1, E5, PAWN);
    ASSERT_TRUE(move_list_contains(e1e5, moves, endp));

    int32_t score = see(&pos, e1e5);
    ASSERT_EQ(score, see_pawn_val);
}


TEST(see_test, xrays)
{
    position_t pos;
    set_pos(&pos, "1k1r3q/1ppn3p/p4b2/4p3/8/P2N2P1/1PP1R1BP/2K1Q3 w - -");

    move_t moves[50], *endp;
    endp = gen_legal_moves(moves, &pos, true, true); 

    move_t d3e5 = to_capture(KNIGHT, D3, E5, PAWN);
    ASSERT_TRUE(move_list_contains(d3e5, moves, endp));

    int32_t score = see(&pos, d3e5);
    ASSERT_EQ(score, see_pawn_val - see_knight_val);
}


TEST(see_test, rook_xrays)
{
    position_t pos;
    set_pos(&pos, "3kr3/8/4p3/8/8/4R3/4R3/4K3 w - -");

    move_t moves[50], *endp;
    endp = gen_legal_moves(moves, &pos, true, true); 

    move_t e3e6 = to_capture(ROOK, E3, E6, PAWN);
    ASSERT_TRUE(move_list_contains(e3e6, moves, endp));

    int32_t score = see(&pos, e3e6);
    ASSERT_EQ(score, see_pawn_val);
}


TEST(see_test, knight_takes_defended_pawn_as_white)
{
    position_t pos;
    set_pos(&pos, "k7/8/5n2/3p4/8/2N2B2/8/K7 w - -");

    move_t moves[50], *endp;
    endp = gen_legal_moves(moves, &pos, true, true); 

    move_t c3d5 = to_capture(KNIGHT, C3, D5, PAWN);
    ASSERT_TRUE(move_list_contains(c3d5, moves, endp));

    int32_t score = see(&pos, c3d5);
    ASSERT_EQ(score, see_pawn_val);
}


TEST(see_test, knight_takes_defended_pawn_as_black)
{
    position_t pos;
    set_pos(&pos, "K7/8/5N2/3P4/8/2n2b2/8/k7 b - -");

    move_t moves[50], *endp;
    endp = gen_legal_moves(moves, &pos, true, true); 

    move_t c3d5 = to_capture(KNIGHT, C3, D5, PAWN);
    ASSERT_TRUE(move_list_contains(c3d5, moves, endp));

    int32_t score = see(&pos, c3d5);
    ASSERT_EQ(score, see_pawn_val);
}


TEST(see_test, crazy_rooks)
{
    position_t pos;
    set_pos(&pos, "2K5/8/8/3pRrRr/8/8/8/2k5 w - -");

    move_t moves[50], *endp;
    endp = gen_legal_moves(moves, &pos, true, true); 

    move_t e5d5 = to_capture(ROOK, E5, D5, PAWN);
    ASSERT_TRUE(move_list_contains(e5d5, moves, endp));

    int32_t score = see(&pos, e5d5);
    ASSERT_EQ(score, see_pawn_val - see_rook_val);
}


TEST(see_test, crazy_rooks2)
{
    position_t pos;
    set_pos(&pos, "2K5/8/8/3pRrR1/8/8/8/2k5 w - -");

    move_t moves[50], *endp;
    endp = gen_legal_moves(moves, &pos, true, true); 

    move_t e5d5 = to_capture(ROOK, E5, D5, PAWN);
    ASSERT_TRUE(move_list_contains(e5d5, moves, endp));

    int32_t score = see(&pos, e5d5);
    ASSERT_EQ(score, see_pawn_val);
}


TEST(see_test, knight_takes_defended_pawn)
{
    position_t pos;
    set_pos(&pos, "1K1k4/8/5n2/3p4/8/1BN5/8/8 w - -");

    move_t moves[50], *endp;
    endp = gen_legal_moves(moves, &pos, true, true); 

    move_t c3d5 = to_capture(KNIGHT, C3, D5, PAWN);
    ASSERT_TRUE(move_list_contains(c3d5, moves, endp));

    int32_t score = see(&pos, c3d5);
    ASSERT_EQ(score, see_pawn_val);
}


TEST(see_test, bishop_takes_defended_pawn)
{
    position_t pos;
    set_pos(&pos, "1K1k4/8/5n2/3p4/8/1BN5/8/8 w - -");

    move_t moves[50], *endp;
    endp = gen_legal_moves(moves, &pos, true, true); 

    move_t b3d5 = to_capture(BISHOP, B3, D5, PAWN);
    ASSERT_TRUE(move_list_contains(b3d5, moves, endp));

    int32_t score = see(&pos, b3d5);
    ASSERT_EQ(score, see_pawn_val-see_bishop_val+see_knight_val);
}


TEST(see_test, knight_takes_defended_pawn_with_crazy_bishops)
{
    position_t pos;
    set_pos(&pos, "1K1k4/8/5n2/3p4/8/2N2B2/6b1/7b w - -");

    move_t moves[50], *endp;
    endp = gen_legal_moves(moves, &pos, true, true); 

    move_t c3d5 = to_capture(KNIGHT, C3, D5, PAWN);
    ASSERT_TRUE(move_list_contains(c3d5, moves, endp));

    int32_t score = see(&pos, c3d5);
    ASSERT_EQ(score, see_pawn_val-see_knight_val);
}


TEST(see_test, non_capturing_promotions)
{
    position_t pos;
    set_pos(&pos, "6RR/4bP2/8/8/5r2/3K4/5p2/4k3 w - -");

    move_t moves[50], *endp;
    endp = gen_legal_moves(moves, &pos, true, true); 

    move_t mv = to_move(PAWN, F7, F8);
    set_promopiece(&mv, QUEEN);

    ASSERT_TRUE(move_list_contains(mv, moves, endp));

    int32_t score = see(&pos, mv);
    ASSERT_EQ(score, see_queen_val-see_pawn_val);
}


TEST(see_test, integration1)
{
    position_t pos;
    set_pos(&pos, "8/8/8/3k1p1p/p1p1PP1P/Pr1p1K2/1P4R1/8 b - -");

    move_t moves[50], *endp;
    endp = gen_legal_moves(moves, &pos, true, true); 

    move_t mv = to_capture(PAWN, F5, E4, PAWN);

    ASSERT_TRUE(move_list_contains(mv, moves, endp));

    int32_t score = see(&pos, mv);
    ASSERT_EQ(score, see_pawn_val);
}


TEST(see_test, integration2)
{
    position_t pos;
    set_pos(&pos, "8/8/5k2/7p/p1p1KP2/r2p4/1p1R3P/8 w - -");

    move_t moves[50], *endp;
    endp = gen_legal_moves(moves, &pos, true, true); 

    move_t mv = to_capture(ROOK, D2, D3, PAWN);

    ASSERT_TRUE(move_list_contains(mv, moves, endp));

    int32_t score = see(&pos, mv);
    ASSERT_EQ(score, see_pawn_val-see_rook_val);
}
