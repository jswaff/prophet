#include "../src/movegen/movegen_internal.h"

#include "prophet/const.h"
#include "prophet/movegen.h"
#include "prophet/position.h"

#include <gtest/gtest.h>

TEST(movegen_utils_test, num_legal_moves)
{
    position_t pos;
    reset_pos(&pos);

    EXPECT_EQ(20U, num_legal_moves(&pos, true, true));
    EXPECT_EQ(20U, num_legal_moves(&pos, false, true));
    EXPECT_EQ(0U, num_legal_moves(&pos, true, false));

    // checkmated position
    set_pos(&pos, "rnb1kbnr/pppp1ppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq -");
    EXPECT_EQ(0U, num_legal_moves(&pos, true, true));    
}

TEST(movegen_utils_test, num_moves_in_list)
{
    position_t pos;
    set_pos(&pos, "8/6bb/8/8/R1pP2k1/4P3/P7/K7 b - d3");

    move_t moves[MAX_MOVES_PER_PLY];
    move_t *endp = gen_legal_moves(moves, &pos, true, true);

    int num_caps, num_noncaps;
    num_moves_in_list(moves, endp, &num_caps, &num_noncaps);

    EXPECT_EQ(1, num_caps); // e.p. capture would leave king in check
    EXPECT_EQ(20, num_noncaps);

    // move the king up so ep capture is possible
    set_pos(&pos, "8/6bb/8/6k1/R1pP4/4P3/P7/K7 b - d3");
    endp = gen_legal_moves(moves, &pos, true, true);
    num_moves_in_list(moves, endp, &num_caps, &num_noncaps);    
    EXPECT_EQ(2, num_caps);
}

TEST(movegen_utils_test, in_check)
{
    position_t pos;

    reset_pos(&pos);
    EXPECT_FALSE(in_check(&pos, WHITE));
    EXPECT_FALSE(in_check(&pos, BLACK));

    // fools mate
    set_pos(&pos, "rnb1kbnr/pppp1ppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq -");
    EXPECT_TRUE(in_check(&pos, WHITE));
    EXPECT_FALSE(in_check(&pos, BLACK));

    // bryne-fischer
    set_pos(&pos, "1Q6/5pk1/2p3p1/1p2N2p/1b5P/1bn5/2r3P1/2K5 w - -");
    EXPECT_TRUE(in_check(&pos, WHITE));
    EXPECT_FALSE(in_check(&pos, BLACK));
}

TEST(movegen_utils_test, is_checkmate)
{
    position_t pos;

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
    position_t pos;

    reset_pos(&pos);
    EXPECT_FALSE(is_checkmate(&pos));

    set_pos(&pos, "8/8/8/6K1/8/1Q6/p7/k7 b - -");
    EXPECT_TRUE(is_stalemate(&pos));

    // burn-pilsbury
    set_pos(&pos, "5k2/5P2/5K2/8/8/8/8/8 b - -");
    EXPECT_TRUE(is_stalemate(&pos));
}

TEST(movegen_utils_test, is_legal_move)
{
    position_t pos;
    reset_pos(&pos);

    move_t legal = to_move(PAWN, E2, E4);
    EXPECT_TRUE(is_legal_move(legal, &pos));

    move_t illegal = to_move(PAWN, E2, E5);
    EXPECT_FALSE(is_legal_move(illegal, &pos));
}

TEST(movegen_utils_test, move_list_contains)
{
    move_t mv_list[5];
    mv_list[0] = to_move(PAWN, E2, E4);
    mv_list[1] = to_move(PAWN, E2, E3);
    mv_list[2] = to_move(KNIGHT, G1, F3);
    mv_list[3] = to_move(BISHOP, B1, D3);
    mv_list[4] = to_move(PAWN, D4, E5);
    set_capture(mv_list+4, PAWN);

    EXPECT_TRUE(move_list_contains(to_move(PAWN, E2, E4), mv_list, mv_list+4));
    EXPECT_FALSE(move_list_contains(to_move(PAWN, D2, D3), mv_list, mv_list+4));
}
