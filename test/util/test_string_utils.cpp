#include <prophet/const.h>
#include <prophet/position/position.h>
#include "../../src/util/string_utils.h"

#include <gtest/gtest.h>

TEST(string_utils_test, str_to_move)
{
    position_t pos;
    reset_pos(&pos);

    // invalid move text
    EXPECT_EQ(str_to_move("blah", &pos), NO_MOVE);

    // legal move from initial position
    EXPECT_EQ(str_to_move("e2e4", &pos), to_move(PAWN, E2, E4));

    // illegal move - wrong color
    EXPECT_EQ(str_to_move("b8a6", &pos), NO_MOVE);

    // no piece on source square
    EXPECT_EQ(str_to_move("e3e4", &pos), NO_MOVE);

    undo_t u;
    apply_move(&pos, to_move(PAWN, E2, E4), &u);

    EXPECT_EQ(str_to_move("g8f6", &pos), to_move(KNIGHT, G8, F6));
    apply_move(&pos, to_move(KNIGHT, G8, F6), &u);

    // illegal move - source occupied
    EXPECT_EQ(str_to_move("b1d2", &pos), NO_MOVE);

    EXPECT_EQ(str_to_move("d2d4", &pos), to_move(PAWN, D2, D4));
    apply_move(&pos, to_move(PAWN, D2, D4), &u);

    // capture
    EXPECT_EQ(str_to_move("f6e4", &pos), to_capture(KNIGHT, F6, E4, PAWN));

    // en passant
    set_pos(&pos, "4k3/8/8/8/3pP3/8/7p/R3K3 b Q e3");
    move_t expected = to_capture(PAWN, D4, E3, PAWN);
    set_epcapture(&expected);
    EXPECT_EQ(str_to_move("d4e3", &pos), expected);
    apply_move(&pos, expected, &u);

    // castle
    expected = to_move(KING, E1, C1);
    set_castle(&expected);
    EXPECT_EQ(str_to_move("e1c1", &pos), expected);

    // pawn promotions
    set_pos(&pos, "2b1k3/PP6/8/3pP3/4P3/8/6P1/4K3 w - d6 0 1");
    move_t mv = to_move(PAWN, A7, A8);
    set_promopiece(&mv, QUEEN);
    EXPECT_EQ(str_to_move("a7a8q", &pos), mv);

    set_promopiece(&mv, ROOK);
    EXPECT_EQ(str_to_move("a7a8r", &pos), mv);

}

TEST(string_utils_test, str_to_sq)
{
    EXPECT_EQ(str_to_sq("-"), NO_SQUARE);
    EXPECT_EQ(str_to_sq("e3"), E3);
    EXPECT_EQ(str_to_sq("a8"), A8);
    EXPECT_EQ(str_to_sq("C5"), C5);
}

TEST(string_utils_test, mv_to_str)
{

    char* buf = move_to_str(to_move(PAWN, E2, E4));
    EXPECT_NE(nullptr, buf);
    EXPECT_EQ(strcmp(buf, "e2e4"), 0);
    free(buf);

    EXPECT_EQ(nullptr, move_to_str(NO_MOVE));

    // promotion
    move_t mv = to_move(PAWN, A7, A8);
    set_promopiece(&mv, BISHOP);
    buf = move_to_str(mv);
    EXPECT_EQ(strcmp(buf, "a7a8b"), 0);
    free(buf);
}

TEST(string_utils_test, mv_line_to_str)
{
    /* empty line */
    move_line_t mv_line;
    mv_line.n = 0;
    char* buf = move_line_to_str(&mv_line);
    EXPECT_EQ(strcmp(buf, ""), 0);
    free(buf);

    /* line of one move */
    mv_line.n = 1;
    mv_line.mv[0] = to_move(PAWN, E2, E4);
    buf = move_line_to_str(&mv_line);
    EXPECT_EQ(strcmp(buf, "e2e4"), 0);
    free(buf);

    /* line of two moves */
    mv_line.n = 2;
    mv_line.mv[1] = to_move(KNIGHT, G8, F6);
    buf = move_line_to_str(&mv_line);
    EXPECT_EQ(strcmp(buf, "e2e4 g8f6"), 0);
    free(buf);

    /* line of three moves */
    mv_line.n = 3;
    mv_line.mv[2] = to_move(PAWN, D2, D3);
    buf = move_line_to_str(&mv_line);
    EXPECT_EQ(strcmp(buf, "e2e4 g8f6 d2d3"), 0);
    free(buf);
}


TEST(string_utils_test, sq_to_str)
{
    char* buf = sq_to_str(E3);
    EXPECT_EQ(strcmp(buf, "e3"), 0);
    free(buf);

    buf = sq_to_str(A8);
    EXPECT_EQ(strcmp(buf, "a8"), 0);
    free(buf);

    buf = sq_to_str(C5);
    EXPECT_EQ(strcmp(buf, "c5"), 0);
    free(buf);

    buf = sq_to_str(NO_SQUARE);
    EXPECT_EQ(buf, nullptr);
}

TEST(string_utils_test, pos_to_str_initial_pos)
{
    position_t pos;
    reset_pos(&pos);

    char expected[] = 
        "r n b q k b n r \n"
        "p p p p p p p p \n"
        "- - - - - - - -    white to move\n"
        "- - - - - - - -    no ep\n"
        "- - - - - - - -    castling rights: KQkq\n"
        "- - - - - - - - \n"
        "P P P P P P P P \n"
        "R N B Q K B N R \n";

    char* buf = pos_to_str(&pos);

    EXPECT_NE(buf, nullptr);

    EXPECT_EQ(strncmp(buf, expected, strlen(expected)), 0);

    free(buf);
}

TEST(string_utils_test, pos_to_str)
{
    position_t pos;
    set_pos(&pos,"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b - e3 0 1");

    char expected[] =
        "r n b q k b n r \n"
        "p p p p p p p p \n"
        "- - - - - - - -    black to move\n"
        "- - - - - - - -    ep sq: e3\n"
        "- - - - P - - -    no castling rights\n"
        "- - - - - - - - \n"
        "P P P P - P P P \n"
        "R N B Q K B N R \n";

    char* buf = pos_to_str(&pos);

    EXPECT_NE(buf, nullptr);

    EXPECT_EQ(strncmp(buf, expected, strlen(expected)), 0);

    free(buf);
}

TEST(string_utils_test, bitmap_to_str_0s)
{
    char expected[] =
        "00000000\n"
        "00000000\n"
        "00000000\n"
        "00000000\n"
        "00000000\n"
        "00000000\n"
        "00000000\n"
        "00000000\n";

    char* buf = bitmap_to_str(0);

    EXPECT_NE(buf, nullptr);

    EXPECT_EQ(strncmp(buf, expected, strlen(expected)), 0);

    free(buf);
}

TEST(string_utils_test, bitmap_to_str_after_e4)
{
    position_t pos;
    set_pos(&pos,"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b - e3 0 1");

    char expected[] =
        "00000000\n"
        "00000000\n"
        "00000000\n"
        "00000000\n"
        "00001000\n"
        "00000000\n"
        "11110111\n"
        "11111111\n";

    char* buf = bitmap_to_str(pos.white_pieces);

    EXPECT_NE(buf, nullptr);

    EXPECT_EQ(strncmp(buf, expected, strlen(expected)), 0);

    free(buf);
}
