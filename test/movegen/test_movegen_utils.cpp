#include <gtest/gtest.h>

#include <prophet/movegen.h>

TEST(movegen_utils_test, is_legal_move)
{
    position pos;
    reset_pos(&pos);

    move_t legal = to_move(PAWN, E2, E4);
    EXPECT_TRUE(is_legal_move(legal, &pos));

    move_t illegal = to_move(PAWN, E2, E5);
    EXPECT_FALSE(is_legal_move(illegal, &pos));
}

TEST(movegen_utils_test, num_legal_moves)
{
    position pos;
    reset_pos(&pos);

    EXPECT_EQ(20U, num_legal_moves(&pos, true, true));
    EXPECT_EQ(20U, num_legal_moves(&pos, false, true));
    EXPECT_EQ(0U, num_legal_moves(&pos, true, false));

    // checkmated position
    set_pos(&pos, "rnb1kbnr/pppp1ppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq -");
    EXPECT_EQ(0U, num_legal_moves(&pos, true, true));    
}

TEST(movegen_utils_test, is_checkmate)
{
    position pos;

    reset_pos(&pos);
    EXPECT_FALSE(is_checkmate(&pos));

    // fools mate
    set_pos(&pos, "rnb1kbnr/pppp1ppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq -");
    EXPECT_TRUE(is_checkmate(&pos));

    // bryne-fischer
    set_pos(&pos, "1Q6/5pk1/2p3p1/1p2N2p/1b5P/1bn5/2r3P1/2K5 w - -");
    EXPECT_TRUE(is_checkmate(&pos));
}

TEST(movegen_utils_test, is_stalemate)
{
    position pos;

    reset_pos(&pos);
    EXPECT_FALSE(is_checkmate(&pos));

    set_pos(&pos, "8/8/8/6K1/8/1Q6/p7/k7 b - -");
    EXPECT_TRUE(is_stalemate(&pos));

    // burn-pilsbury
    set_pos(&pos, "5k2/5P2/5K2/8/8/8/8/8 b - -");
    EXPECT_TRUE(is_stalemate(&pos));
}