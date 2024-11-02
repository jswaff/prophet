#include "movegen/movegen_internal.h"

#include "prophet/position.h"

#include <gtest/gtest.h>

#include <stdint.h>

static uint64_t test_case(const char *fen, int depth);

/**
 * Just a few basic tests to give some quick validation of the move generation 
 * functions. This suite is not meant as a performance metric.
 *
 * Many of these test positions came from Arasan.
 */
TEST(perft_test, perft_initial_pos)
{
    position_t pos;
    reset_pos(&pos);

    EXPECT_EQ(perft(&pos, 1), (uint64_t)20);
    EXPECT_EQ(perft(&pos, 2), (uint64_t)400);
    EXPECT_EQ(perft(&pos, 3), (uint64_t)8902);
    EXPECT_EQ(perft(&pos, 4), (uint64_t)197281);
}

TEST(perft_test, illegal_ep)
{
    EXPECT_EQ(
        test_case("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1", 6), (uint64_t)1134888);
    EXPECT_EQ(
        test_case("8/8/8/8/k1p4R/8/3P4/3K4 w - - 0 1", 6), (uint64_t)1134888);

    EXPECT_EQ(
        test_case("8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1", 6), (uint64_t)1015133);
    EXPECT_EQ(
        test_case("8/b2p2k1/8/2P5/8/4K3/8/8 b - - 0 1", 6), (uint64_t)1015133);
}

TEST(perft_test, ep_capture_checks_opponent)
{
    EXPECT_EQ(
        test_case("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1", 6), 
            (uint64_t)1440467);
    EXPECT_EQ(
        test_case("8/5k2/8/2Pp4/2B5/1K6/8/8 w - d6 0 1", 6), 
            (uint64_t)1440467);
}

TEST(perft_test, short_castle_gives_check)
{
    EXPECT_EQ(
        test_case("5k2/8/8/8/8/8/8/4K2R w K - 0 1", 6), (uint64_t)661072);
    EXPECT_EQ(
        test_case("4k2r/8/8/8/8/8/8/5K2 b k - 0 1", 6), (uint64_t)661072);
}

TEST(perft_test, long_castle_gives_check)
{
    EXPECT_EQ(
        test_case("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1", 6), (uint64_t)803711);
    EXPECT_EQ(
        test_case("r3k3/8/8/8/8/8/8/3K4 b q - 0 1", 6), (uint64_t)803711);
}

TEST(perft_test, losing_castling_rights) // no pawns
{
    EXPECT_EQ(
        test_case("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1", 4), 
            (uint64_t)1274206);
    EXPECT_EQ(
        test_case("r3k2r/7b/8/8/8/8/1B4BQ/R3K2R b KQkq - 0 1", 4), 
            (uint64_t)1274206);
}

TEST(perft_test, castling_prevented)
{
    EXPECT_EQ(
        test_case("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1", 4), 
            (uint64_t)1720476);
    EXPECT_EQ(
        test_case("r3k2r/8/5Q2/8/8/3q4/8/R3K2R w KQkq - 0 1", 4), 
            (uint64_t)1720476);
}

TEST(perft_test, promote_out_of_check)
{
    EXPECT_EQ(
        test_case("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1", 6),(uint64_t)3821001);
    EXPECT_EQ(
        test_case("3K4/8/8/8/8/8/4p3/2k2R2 b - - 0 1", 6),(uint64_t)3821001);
}

TEST(perft_test, discovered_check)
{
    EXPECT_EQ(
        test_case("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1", 5), 
            (uint64_t)1004658);
    EXPECT_EQ(
        test_case("5K2/8/1Q6/2N5/8/1p2k3/8/8 w - - 0 1", 5), 
            (uint64_t)1004658);
}

TEST(perft_test, promote_to_give_check)
{
    EXPECT_EQ(
        test_case("4k3/1P6/8/8/8/8/K7/8 w - - 0 1", 6), (uint64_t)217342);
    EXPECT_EQ(
        test_case("8/k7/8/8/8/8/1p6/4K3 b - - 0 1", 6), (uint64_t)217342);
}

TEST(perft_test, underpromote_to_check)
{
    EXPECT_EQ(test_case("8/P1k5/K7/8/8/8/8/8 w - - 0 1", 6), (uint64_t)92683);
    EXPECT_EQ(test_case("8/8/8/8/8/k7/p1K5/8 b - - 0 1", 6), (uint64_t)92683);
}

TEST(perft_test, self_stalemate)
{
    EXPECT_EQ(test_case("K1k5/8/P7/8/8/8/8/8 w - - 0 1", 6), (uint64_t)2217);
    EXPECT_EQ(test_case("8/8/8/8/8/p7/8/k1K5 b - - 0 1", 6), (uint64_t)2217);
}

TEST(perft_test, stalemate_checkmate)
{
    EXPECT_EQ(
        test_case("8/k1P5/8/1K6/8/8/8/8 w - - 0 1", 7), (uint64_t)567584);
    EXPECT_EQ(
        test_case("8/8/8/8/1k6/8/K1p5/8 b - - 0 1", 7), (uint64_t)567584);
}

TEST(perft_test, double_check)
{
    EXPECT_EQ(
        test_case("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1", 4), (uint64_t)23527);
    EXPECT_EQ(
        test_case("8/5k2/8/5N2/5Q2/2K5/8/8 w - - 0 1", 4), (uint64_t)23527);
}

TEST(perft_test, perft_gauntlet)
{
    EXPECT_EQ(
        test_case(
            "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 
            3), 
        (uint64_t)97862);
    EXPECT_EQ(
        test_case(
            "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 6), 
        (uint64_t)11030083L);
    EXPECT_EQ(
        test_case(
            "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 
            5), 
        (uint64_t)15833292L);
    EXPECT_EQ(
        test_case(
            "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 4), 
        (uint64_t)2103487);
    EXPECT_EQ(
        test_case(
            "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 
            4),
        (uint64_t)3894594L);
}

static uint64_t test_case(const char *fen, int depth)
{
    position_t pos;
    set_pos(&pos, fen);
    return perft(&pos, depth);
}
