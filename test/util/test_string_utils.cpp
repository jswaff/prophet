#include <gtest/gtest.h>

#include <prophet/position/position.h>
#include <prophet/util/string_utils.h>

TEST(string_utils_test, str_to_sq)
{
    EXPECT_EQ(str_to_sq("-"), NO_SQUARE);
    EXPECT_EQ(str_to_sq("e3"), E3);
    EXPECT_EQ(str_to_sq("a8"), A8);
    EXPECT_EQ(str_to_sq("C5"), C5);
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
