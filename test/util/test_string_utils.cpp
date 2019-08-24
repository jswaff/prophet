#include <prophet/const.h>
#include <prophet/position/position.h>
#include <prophet/util/string_utils.h>

#include <gtest/gtest.h>

TEST(string_utils_test, str_to_move)
{
    position pos;
    reset_pos(&pos);

    // legal moves from initial position
    EXPECT_EQ(str_to_move("e2e4", &pos), to_move(PAWN, E2, E4));
    EXPECT_EQ(str_to_move("b8a6", &pos), to_move(KNIGHT, B8, A6));

    // no piece on source square
    EXPECT_EQ(str_to_move("e3e4", &pos), NO_MOVE);

    // pawn promotions
    set_pos(&pos, "2b1k3/PP6/8/3pP3/4P3/8/6P1/4K3 w - d6 0 1");
    move_t mv = to_move(PAWN, A7, A8);
    set_promopiece(&mv, QUEEN);
    EXPECT_EQ(str_to_move("a7a8q", &pos), mv);

    set_promopiece(&mv, ROOK);
    EXPECT_EQ(str_to_move("a7a8r", &pos), mv);

    // invalid move text
    EXPECT_EQ(str_to_move("blah", &pos), NO_MOVE);
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
    position pos;
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
    position pos;
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
