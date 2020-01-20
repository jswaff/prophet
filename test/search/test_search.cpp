#include <prophet/const.h>
#include <prophet/eval.h>
#include <prophet/search.h>

#include <gtest/gtest.h>

TEST(search_test, depth0_no_bounds)
{
    position_t pos;
    reset_pos(&pos);
    stats_t stats;

    ASSERT_EQ(eval(&pos, false), search(&pos, 0, -INF, INF, &stats));
}

TEST(search_test, mate_in_1)
{
    position_t pos;
    set_pos(&pos, "4k3/8/3Q4/2B5/8/8/1K6/8 w - -");
    stats_t stats;

    ASSERT_EQ(CHECKMATE-1, search(&pos, 2, -INF, INF, &stats));
}

TEST(search_test, mate_in_2)
{
    position_t pos;
    set_pos(&pos, "r1bq2r1/b4pk1/p1pp1p2/1p2pP2/1P2P1PB/3P4/1PPQ2P1/R3K2R w - -");
    stats_t stats;

    ASSERT_EQ(CHECKMATE-3, search(&pos, 4, -INF, INF, &stats));
}

TEST(search_test, mate_in_3)
{
    position_t pos;
    set_pos(&pos, "r5rk/5p1p/5R2/4B3/8/8/7P/7K w - -");
    stats_t stats;

    ASSERT_EQ(CHECKMATE-5, search(&pos, 6, -INF, INF, &stats));
}

TEST(search_test, stalemate)
{
    position_t pos;
    set_pos(&pos, "8/6p1/5p2/5k1K/7P/8/8/8 w - -");
    stats_t stats;

    ASSERT_EQ(0, search(&pos, 1, -INF, INF, &stats));
}
