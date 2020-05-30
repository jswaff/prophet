#include <prophet/const.h>
#include <prophet/eval.h>
#include <prophet/parameters.h>
#include <prophet/search.h>

#include <prophet/util/string_utils.h>

#include <gtest/gtest.h>

extern move_line_t last_pv;
extern bool stop_search;

static void pv_cb(move_line_t* UNUSED(pv), int32_t UNUSED(depth), 
    int32_t UNUSED(score), uint64_t UNUSED(elapased), 
    uint64_t UNUSED(num_nodes))
{
    /* no op */
}

TEST(search_test, depth0_no_bounds)
{
    position_t pos;
    reset_pos(&pos);
    move_line_t pv;
    stats_t stats;
    move_t moves[100];
    undo_t undos[1];
    memset(&last_pv, 0, sizeof(move_line_t));
    stop_search = false;

    ASSERT_EQ(eval(&pos, false), search(&pos, &pv, 0, -INF, INF, moves, undos, 
        &stats, pv_cb, 0, 0));
    ASSERT_EQ(0, pv.n);
    ASSERT_EQ(1U, stats.nodes);
}

TEST(search_test, mate_in_1)
{
    position_t pos;
    set_pos(&pos, "4k3/8/3Q4/2B5/8/8/1K6/8 w - -");
    move_line_t pv;
    stats_t stats;
    move_t moves[200];
    undo_t undos[2];
    memset(&last_pv, 0, sizeof(move_line_t));
    stop_search = false;

    ASSERT_EQ(CHECKMATE-1, search(&pos, &pv, 2, -INF, INF, moves, undos, 
        &stats, pv_cb, 0, 0));
    ASSERT_EQ(1, pv.n);
    ASSERT_EQ(to_move(QUEEN, D6, E7), pv.mv[0]);
}

TEST(search_test, mate_in_2)
{
    position_t pos;
    set_pos(&pos, "r1bq2r1/b4pk1/p1pp1p2/1p2pP2/1P2P1PB/3P4/1PPQ2P1/R3K2R w - -");
    move_line_t pv;
    stats_t stats;
    move_t moves[400];
    undo_t undos[4];
    memset(&last_pv, 0, sizeof(move_line_t));
    stop_search = false;

    ASSERT_EQ(CHECKMATE-3, search(&pos, &pv, 4, -INF, INF, moves, undos, 
        &stats, pv_cb, 0, 0));
    ASSERT_EQ(3, pv.n);
    ASSERT_EQ(to_move(QUEEN, D2, H6), pv.mv[0]);
    ASSERT_EQ(to_capture(KING, G7, H6, QUEEN), pv.mv[1]);
    ASSERT_EQ(to_capture(BISHOP, H4, F6, PAWN), pv.mv[2]);
}

TEST(search_test, mate_in_3)
{
    position_t pos;
    set_pos(&pos, "r5rk/5p1p/5R2/4B3/8/8/7P/7K w - -");
    move_line_t pv;
    stats_t stats;
    move_t moves[600];
    undo_t undos[6];
    memset(&last_pv, 0, sizeof(move_line_t));
    stop_search = false;

    ASSERT_EQ(CHECKMATE-5, search(&pos, &pv, 6, -INF, INF, moves, undos, 
        &stats, pv_cb, 0, 0));
    ASSERT_EQ(5, pv.n);
    ASSERT_EQ(to_move(ROOK, F6, A6), pv.mv[0]);
    ASSERT_EQ(to_move(PAWN, F7, F6), pv.mv[1]);
    ASSERT_EQ(to_capture(BISHOP, E5, F6, PAWN), pv.mv[2]);
    ASSERT_EQ(to_move(ROOK, G8, G7), pv.mv[3]);
    ASSERT_EQ(to_capture(ROOK, A6, A8, ROOK), pv.mv[4]);
}

TEST(search_test, stalemate)
{
    position_t pos;
    set_pos(&pos, "8/6p1/5p2/5k1K/7P/8/8/8 w - -");
    move_line_t pv;
    stats_t stats;
    move_t moves[100];
    undo_t undos[1];
    memset(&last_pv, 0, sizeof(move_line_t));
    stop_search = false;

    ASSERT_EQ(0, search(&pos, &pv, 1, -INF, INF, moves, undos, &stats, pv_cb, 
        0, 0));
    ASSERT_EQ(0, pv.n);
}


TEST(search_test, stop_search)
{
    position_t pos;
    reset_pos(&pos);
    move_line_t pv;
    stats_t stats;
    move_t moves[100];
    undo_t undos[3];
    memset(&last_pv, 0, sizeof(move_line_t));
    stop_search = true;

    // start a depth 3 search
    ASSERT_EQ(eval(&pos, false), search(&pos, &pv, 3, -INF, INF, moves, undos, 
        &stats, pv_cb, 0, 0));

    // visit one node per depth, down the left side of the tree
    ASSERT_EQ(4U, stats.nodes);

    // the PV should not have been updated
    ASSERT_EQ(0, pv.n);
}
