#include "movegen/movegen_internal.h"

#include "prophet/move.h"
#include "prophet/position.h"

#include "position/position_internal.h"

#include <gtest/gtest.h>


TEST(movegen_test, pawn_moves_initial_position)
{
    position_t pos;
    reset_pos(&pos);

    move_t moves[20], *mp;

    mp = gen_pawn_moves(moves, &pos, true, true);

    EXPECT_EQ(mp-moves, 16);
    EXPECT_TRUE(move_list_contains(to_move(PAWN, A2, A3), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, A2, A4), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, B2, B3), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, B2, B4), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, C2, C3), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, C2, C4), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, D2, D3), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, D2, D4), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, E2, E3), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, E2, E4), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, F2, F3), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, F2, F4), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, G2, G3), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, G2, G4), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, H2, H3), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, H2, H4), moves, mp));

    // from black
    pos.player = BLACK;

    mp = gen_pawn_moves(moves, &pos, true, true);

    EXPECT_EQ(mp-moves, 16);
    EXPECT_TRUE(move_list_contains(to_move(PAWN, A7, A6), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, A7, A5), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, B7, B6), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, B7, B5), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, C7, C6), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, C7, C5), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, D7, D6), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, D7, D5), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, E7, E6), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, E7, E5), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, F7, F6), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, F7, F5), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, G7, G6), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, G7, G5), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, H7, H6), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, H7, H5), moves, mp));
}

TEST(movegen_test, pawn_moves)
{
    position_t pos;
    set_pos(&pos, "2b1k3/PP6/8/3pP3/4P3/8/6P1/4K3 w - d6 0 1");
    move_t moves[20], *mp, mv;

    mp = gen_pawn_moves(moves, &pos, true, true);

    EXPECT_EQ(mp - moves, 17);
    EXPECT_TRUE(move_list_contains(to_move(PAWN, G2, G3), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, G2, G4), moves, mp));

    mv = to_move(PAWN,A7,A8);
    set_promopiece(&mv,QUEEN);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    set_promopiece(&mv,ROOK);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    set_promopiece(&mv,BISHOP);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    set_promopiece(&mv, KNIGHT);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    mv = to_move(PAWN, B7, B8);
    set_promopiece(&mv, QUEEN);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    set_promopiece(&mv, ROOK);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    set_promopiece(&mv, BISHOP);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    set_promopiece(&mv, KNIGHT);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    mv = to_capture(PAWN, B7, C8, BISHOP);
    set_promopiece(&mv,QUEEN);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    set_promopiece(&mv, ROOK);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    set_promopiece(&mv, BISHOP);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    set_promopiece(&mv, KNIGHT);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    mv = to_capture(PAWN, E4, D5, PAWN);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    EXPECT_TRUE(move_list_contains(to_move(PAWN, E5, E6), moves, mp));

    mv = to_move(PAWN, E5, D6);
    set_epcapture(&mv);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    // from black
    set_pos(&pos, 
        "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    mp = gen_pawn_moves(moves, &pos, true, true);

    EXPECT_EQ(mp - moves, 16);

    EXPECT_TRUE(move_list_contains(to_move(PAWN, D7, D6), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, D7, D5), moves, mp));
}

TEST(movegen_test, pawn_captures)
{
    position_t pos;
    set_pos(&pos, "2b1k3/PP6/8/3pP3/4P3/8/6P1/4K3 w - d6 0 1");
    move_t moves[20], *mp, mv;

    mp = gen_pawn_moves(moves, &pos, true, false);

    EXPECT_EQ(mp - moves, 14);

    mv = to_move(PAWN,A7,A8);
    set_promopiece(&mv,QUEEN);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    set_promopiece(&mv,ROOK);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    set_promopiece(&mv,BISHOP);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    set_promopiece(&mv, KNIGHT);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    mv = to_move(PAWN, B7, B8);
    set_promopiece(&mv, QUEEN);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    set_promopiece(&mv, ROOK);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    set_promopiece(&mv, BISHOP);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    set_promopiece(&mv, KNIGHT);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    mv = to_capture(PAWN, B7, C8, BISHOP);
    set_promopiece(&mv,QUEEN);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    set_promopiece(&mv, ROOK);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    set_promopiece(&mv, BISHOP);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    set_promopiece(&mv, KNIGHT);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    mv = to_capture(PAWN, E4, D5, PAWN);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    mv = to_move(PAWN, E5, D6);
    set_epcapture(&mv);
    EXPECT_TRUE(move_list_contains(mv, moves, mp));

    // from black
    set_pos(&pos, 
        "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    mp = gen_pawn_moves(moves, &pos, true, false);

    EXPECT_EQ(mp - moves, 0);
}

TEST(movegen_test, pawn_noncaptures)
{
    position_t pos;
    set_pos(&pos, "2b1k3/PP6/8/3pP3/4P3/8/6P1/4K3 w - d6 0 1");
    move_t moves[20], *mp;

    mp = gen_pawn_moves(moves, &pos, false, true);

    EXPECT_EQ(mp - moves, 3);

    EXPECT_TRUE(move_list_contains(to_move(PAWN, G2, G3), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, G2, G4), moves, mp));
    EXPECT_TRUE(move_list_contains(to_move(PAWN, E5, E6), moves, mp));

    // from black
    set_pos(&pos, 
        "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    mp = gen_pawn_moves(moves, &pos, false, true);

    EXPECT_EQ(mp - moves, 16);
}
