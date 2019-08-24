#include <prophet/const.h>
#include <prophet/position/move.h>

#include <gtest/gtest.h>

TEST(move_test, to_move)
{
    move_t mv = to_move(PAWN,E2,E4);
    EXPECT_EQ(get_piece(mv), PAWN);
    EXPECT_EQ(get_from_sq(mv), E2);
    EXPECT_EQ(get_to_sq(mv), E4);
    EXPECT_FALSE(is_epcapture(mv));
    EXPECT_FALSE(is_capture(mv));
    EXPECT_FALSE(is_castle(mv));
    EXPECT_EQ(get_promopiece(mv), NO_PIECE);
}

TEST(move_test, set_promopiece)
{
    move_t mv = to_move(PAWN,B2,B1);
    EXPECT_EQ(get_piece(mv), PAWN);
    EXPECT_EQ(get_from_sq(mv), B2);
    EXPECT_EQ(get_to_sq(mv), B1);
    EXPECT_FALSE(is_epcapture(mv));
    EXPECT_FALSE(is_capture(mv));
    EXPECT_FALSE(is_castle(mv));
    EXPECT_EQ(get_promopiece(mv), NO_PIECE);

    set_promopiece(&mv,ROOK);
    EXPECT_EQ(get_piece(mv), PAWN);
    EXPECT_EQ(get_from_sq(mv), B2);
    EXPECT_EQ(get_to_sq(mv), B1);
    EXPECT_FALSE(is_epcapture(mv));
    EXPECT_FALSE(is_capture(mv));
    EXPECT_FALSE(is_castle(mv));
    EXPECT_EQ(get_promopiece(mv), ROOK);

    set_promopiece(&mv,BISHOP);
    EXPECT_EQ(get_promopiece(mv), BISHOP);

    set_promopiece(&mv,QUEEN);
    EXPECT_EQ(get_promopiece(mv), QUEEN);

    set_promopiece(&mv,KNIGHT);
    EXPECT_EQ(get_promopiece(mv), KNIGHT);

    set_promopiece(&mv,NO_PIECE);
    EXPECT_EQ(get_piece(mv), PAWN);
    EXPECT_EQ(get_from_sq(mv), B2);
    EXPECT_EQ(get_to_sq(mv), B1);
    EXPECT_FALSE(is_epcapture(mv));
    EXPECT_FALSE(is_capture(mv));
    EXPECT_FALSE(is_castle(mv));
    EXPECT_EQ(get_promopiece(mv), NO_PIECE);

    ///////
    mv = to_capture(PAWN,B2,A1,ROOK);
    set_promopiece(&mv,QUEEN);
    EXPECT_EQ(get_piece(mv), PAWN);
    EXPECT_EQ(get_from_sq(mv), B2);
    EXPECT_EQ(get_to_sq(mv), A1);
    EXPECT_FALSE(is_epcapture(mv));
    EXPECT_TRUE(is_capture(mv));
    EXPECT_FALSE(is_castle(mv));
    EXPECT_EQ(get_promopiece(mv), QUEEN);
}

TEST(move_test, set_capture)
{
    move_t mv = to_move(BISHOP,F3,C6);
    EXPECT_EQ(get_piece(mv), BISHOP);
    EXPECT_EQ(get_from_sq(mv), F3);
    EXPECT_EQ(get_to_sq(mv), C6);
    EXPECT_FALSE(is_epcapture(mv));
    EXPECT_FALSE(is_capture(mv));
    EXPECT_EQ(get_captured_piece(mv), NO_PIECE);
    EXPECT_FALSE(is_castle(mv));
    EXPECT_EQ(get_promopiece(mv), NO_PIECE);

    set_capture(&mv,PAWN);
    EXPECT_EQ(get_piece(mv), BISHOP);
    EXPECT_EQ(get_from_sq(mv), F3);
    EXPECT_EQ(get_to_sq(mv), C6);
    EXPECT_FALSE(is_epcapture(mv));
    EXPECT_TRUE(is_capture(mv));
    EXPECT_EQ(get_captured_piece(mv), PAWN);
    EXPECT_FALSE(is_castle(mv));
    EXPECT_EQ(get_promopiece(mv), NO_PIECE);
}

TEST(move_test, set_epcapture)
{
    move_t mv = to_move(PAWN,D4,C3);
    set_epcapture(&mv);
    EXPECT_EQ(get_piece(mv), PAWN);
    EXPECT_EQ(get_from_sq(mv), D4);
    EXPECT_EQ(get_to_sq(mv), C3);
    EXPECT_TRUE(is_epcapture(mv));
    EXPECT_TRUE(is_capture(mv));
    EXPECT_EQ(get_captured_piece(mv), PAWN);
    EXPECT_FALSE(is_castle(mv));
    EXPECT_EQ(get_promopiece(mv), NO_PIECE);
}

TEST(move_test, set_castle)
{
    move_t mv = to_move(KING,E1,H1);
    set_castle(&mv);
    EXPECT_EQ(get_piece(mv), KING);
    EXPECT_EQ(get_from_sq(mv), E1);
    EXPECT_EQ(get_to_sq(mv), H1);
    EXPECT_FALSE(is_epcapture(mv));
    EXPECT_FALSE(is_capture(mv));
    EXPECT_TRUE(is_castle(mv));
    EXPECT_EQ(get_promopiece(mv), NO_PIECE);
}

TEST(move_test, move_score)
{
    move_t mv = to_move(PAWN,E2,E4);
    EXPECT_EQ(get_move_score(mv), 0);

    set_move_score(&mv,100);
    EXPECT_EQ(get_move_score(mv), 100);

    set_move_score(&mv,-9);
    EXPECT_EQ(get_move_score(mv), -9);

    set_move_score(&mv,CHECKMATE);
    EXPECT_EQ(get_move_score(mv), CHECKMATE);

    set_move_score(&mv,-CHECKMATE);
    EXPECT_EQ(get_move_score(mv), -CHECKMATE);

    set_move_score(&mv,-CHECKMATE+10);
    EXPECT_EQ(get_move_score(mv), -CHECKMATE+10);

    // make sure we haven't corrupted the move itself
    EXPECT_EQ(get_piece(mv), PAWN);
    EXPECT_EQ(get_from_sq(mv), E2);
    EXPECT_EQ(get_to_sq(mv), E4);
}

TEST(move_test, clear_score)
{
    move_t mv = to_move(PAWN,E2,E4);
    EXPECT_EQ(get_move_score(mv), 0);

    set_move_score(&mv, 100);
    EXPECT_EQ(get_move_score(mv), 100);
    move_t cmv = clear_score(mv);
    EXPECT_EQ(get_move_score(cmv), 0);

    set_move_score(&mv, -CHECKMATE);
    EXPECT_EQ(get_move_score(mv), -CHECKMATE);
    cmv = clear_score(mv);
    EXPECT_EQ(get_move_score(cmv), 0);
}
