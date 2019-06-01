#include <gtest/gtest.h>

#include <prophet/position/position.h>

#include "../../src/position/position_internal.h"

TEST(pos_test, apply_move_sequence1)
{
    position pos1;
    reset_pos(&pos1);
    undo u;

    move mv = to_move(PAWN, E2, E4);
    apply_move(&pos1, mv, &u);
    position pos2;
    set_pos(&pos2, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    EXPECT_TRUE(pos_equals(&pos1, &pos2, true));

    mv = to_move(PAWN, C7, C5);
    apply_move(&pos1, mv, &u);
    set_pos(&pos2, "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");
    EXPECT_TRUE(pos_equals(&pos1, &pos2, true));

    mv = to_move(KNIGHT, G1, F3);
    apply_move(&pos1, mv, &u);
    set_pos(&pos2, "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
    EXPECT_TRUE(pos_equals(&pos1, &pos2, true));
    EXPECT_EQ(pos2.fifty_counter, 1U);

    mv = to_move(QUEEN, D8, A5);
    apply_move(&pos1, mv, &u);
    set_pos(&pos2, "rnb1kbnr/pp1ppppp/8/q1p5/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 2 3");
    EXPECT_TRUE(pos_equals(&pos1, &pos2, true));
    EXPECT_EQ(pos2.fifty_counter, 2U);

    mv = to_move(BISHOP, F1, E2);
    apply_move(&pos1, mv, &u);
    set_pos(&pos2, "rnb1kbnr/pp1ppppp/8/q1p5/4P3/5N2/PPPPBPPP/RNBQK2R b KQkq - 3 3");
    EXPECT_TRUE(pos_equals(&pos1, &pos2, true));

    mv = to_capture(QUEEN, A5, D2, PAWN);
    apply_move(&pos1, mv, &u);
    set_pos(&pos2, "rnb1kbnr/pp1ppppp/8/2p5/4P3/5N2/PPPqBPPP/RNBQK2R w KQkq - 0 4");
    EXPECT_TRUE(pos_equals(&pos1, &pos2, true));

    mv = to_capture(KNIGHT, B1, D2, QUEEN);
    apply_move(&pos1, mv, &u);
    set_pos(&pos2, "rnb1kbnr/pp1ppppp/8/2p5/4P3/5N2/PPPNBPPP/R1BQK2R b KQkq - 0 4");
    EXPECT_TRUE(pos_equals(&pos1, &pos2, true));

    mv = to_move(PAWN, C5, C4);
    apply_move(&pos1, mv, &u);
    set_pos(&pos2, "rnb1kbnr/pp1ppppp/8/8/2p1P3/5N2/PPPNBPPP/R1BQK2R w KQkq - 0 5");
    EXPECT_TRUE(pos_equals(&pos1, &pos2, true));

    mv = to_move(KING, E1, G1);
    set_castle(&mv);
    apply_move(&pos1, mv, &u);
    set_pos(&pos2, "rnb1kbnr/pp1ppppp/8/8/2p1P3/5N2/PPPNBPPP/R1BQ1RK1 b kq - 0 5");
    EXPECT_TRUE(pos_equals(&pos1, &pos2, true));

    mv = to_move(KING, E8, D8);
    apply_move(&pos1, mv, &u);
    set_pos(&pos2, "rnbk1bnr/pp1ppppp/8/8/2p1P3/5N2/PPPNBPPP/R1BQ1RK1 w - - 1 6");
    EXPECT_TRUE(pos_equals(&pos1, &pos2, true));

    mv = to_move(PAWN, B2, B4);
    apply_move(&pos1, mv, &u);
    set_pos(&pos2, "rnbk1bnr/pp1ppppp/8/8/1Pp1P3/5N2/P1PNBPPP/R1BQ1RK1 b - b3 0 6");
    EXPECT_TRUE(pos_equals(&pos1, &pos2, true));

    mv = to_move(PAWN, C4, B3);
    set_epcapture(&mv);
    apply_move(&pos1, mv, &u);
    set_pos(&pos2, "rnbk1bnr/pp1ppppp/8/8/4P3/1p3N2/P1PNBPPP/R1BQ1RK1 w - - 0 7");
    EXPECT_TRUE(pos_equals(&pos1, &pos2, true));

    mv = to_move(ROOK, F1, E1);
    apply_move(&pos1, mv, &u);
    set_pos(&pos2, "rnbk1bnr/pp1ppppp/8/8/4P3/1p3N2/P1PNBPPP/R1BQR1K1 b - - 1 7");
    EXPECT_TRUE(pos_equals(&pos1, &pos2, true));

    mv = to_move(PAWN, B3, B2);
    apply_move(&pos1, mv, &u);
    set_pos(&pos2, "rnbk1bnr/pp1ppppp/8/8/4P3/5N2/PpPNBPPP/R1BQR1K1 w - - 0 8");
    EXPECT_TRUE(pos_equals(&pos1, &pos2, true));

    mv = to_move(KING, G1, H1);
    apply_move(&pos1, mv, &u);
    set_pos(&pos2, "rnbk1bnr/pp1ppppp/8/8/4P3/5N2/PpPNBPPP/R1BQR2K b - - 1 8");
    EXPECT_TRUE(pos_equals(&pos1, &pos2, true));

    mv = to_capture(PAWN, B2, A1, ROOK);
    set_promopiece(&mv, KNIGHT);
    apply_move(&pos1, mv, &u);
    set_pos(&pos2, "rnbk1bnr/pp1ppppp/8/8/4P3/5N2/P1PNBPPP/n1BQR2K w - - 0 9");
    EXPECT_TRUE(pos_equals(&pos1, &pos2, true));
}

// this test more focused on castling rights
TEST(pos_test, apply_move_sequence2)
{
    position pos;
    set_pos(&pos,"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
    undo u;

    EXPECT_TRUE(can_castle_wk(&pos));
    EXPECT_TRUE(can_castle_wq(&pos));
    EXPECT_TRUE(can_castle_bk(&pos));
    EXPECT_TRUE(can_castle_bq(&pos));

    // queenside black rook takes white rook removed qside castling options
    move mv = to_capture(ROOK, A1, A8, ROOK);
    apply_move(&pos, mv, &u);
    EXPECT_TRUE(can_castle_wk(&pos));
    EXPECT_FALSE(can_castle_wq(&pos));
    EXPECT_TRUE(can_castle_bk(&pos));
    EXPECT_FALSE(can_castle_bq(&pos));

    // moving the black king removes bk castling option
    mv = to_move(KING, E8, E7);
    apply_move(&pos, mv, &u);
    EXPECT_TRUE(can_castle_wk(&pos));
    EXPECT_FALSE(can_castle_wq(&pos));
    EXPECT_FALSE(can_castle_bk(&pos));
    EXPECT_FALSE(can_castle_bq(&pos));

    // moving the wk rook removes wk castling option
    mv = to_move(ROOK, H1, H7);
    apply_move(&pos, mv, &u);
    EXPECT_EQ(pos.castling_rights, 0U);
}

