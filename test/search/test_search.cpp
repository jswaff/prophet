#include "prophet/search.h"

#include <prophet/const.h>
#include <prophet/hash.h>

#include "parameters.h"
#include "eval/eval_internal.h"
#include "util/time.h"
#include "util/string_utils.h"

#include <gtest/gtest.h>

extern move_line_t last_pv;
extern bool stop_search;

TEST(search_test, depth0_no_bounds)
{
    clear_hash_tables();
    position_t pos;
    reset_pos(&pos);
    move_line_t pv;
    stats_t stats;
    memset(&stats, 0, sizeof(stats_t));
    search_options_t opts;
    memset(&opts, 0, sizeof(search_options_t));
    move_t moves[100];
    undo_t undos[1];
    memset(&last_pv, 0, sizeof(move_line_t));
    stop_search = false;

    ASSERT_EQ(
        eval(&pos, false, false), 
        search(&pos, &pv, 0, -CHECKMATE, CHECKMATE, moves, undos, &stats, &opts));
    ASSERT_EQ(0, pv.n);
    ASSERT_EQ(0U, stats.nodes);
    ASSERT_EQ(1U, stats.qnodes);
}

TEST(search_test, mate_in_1)
{
    clear_hash_tables();
    position_t pos;
    set_pos(&pos, "4k3/8/3Q4/2B5/8/8/1K6/8 w - -");

    move_line_t pv;
    stats_t stats;
    memset(&stats, 0, sizeof(stats_t));
    search_options_t opts;
    memset(&opts, 0, sizeof(search_options_t));
    move_t moves[200];
    undo_t undos[10];
    memset(&last_pv, 0, sizeof(move_line_t));
    stop_search = false;

    ASSERT_EQ(CHECKMATE-1, search(&pos, &pv, 2, -CHECKMATE, CHECKMATE, moves, undos, 
        &stats, &opts));
    ASSERT_EQ(1, pv.n);
    ASSERT_EQ(to_move(QUEEN, D6, E7), pv.mv[0]);
}

TEST(search_test, mate_in_2)
{
    clear_hash_tables();
    position_t pos;
    set_pos(&pos, "r1bq2r1/b4pk1/p1pp1p2/1p2pP2/1P2P1PB/3P4/1PPQ2P1/R3K2R w - -");
    move_line_t pv;
    stats_t stats;
    memset(&stats, 0, sizeof(stats_t));
    search_options_t opts;
    memset(&opts, 0, sizeof(search_options_t));
    move_t moves[400];
    undo_t undos[20];
    memset(&last_pv, 0, sizeof(move_line_t));
    stop_search = false;

    ASSERT_EQ(CHECKMATE-3, search(&pos, &pv, 4, -CHECKMATE, CHECKMATE, moves, undos, 
        &stats, &opts));
    ASSERT_EQ(3, pv.n);
    ASSERT_EQ(to_move(QUEEN, D2, H6), pv.mv[0]);
    ASSERT_EQ(to_capture(KING, G7, H6, QUEEN), pv.mv[1]);
    ASSERT_EQ(to_capture(BISHOP, H4, F6, PAWN), pv.mv[2]);
}

TEST(search_test, mate_in_3)
{
    clear_hash_tables();
    position_t pos;
    set_pos(&pos, "r5rk/5p1p/5R2/4B3/8/8/7P/7K w - -");
    move_line_t pv;
    stats_t stats;
    memset(&stats, 0, sizeof(stats_t));
    search_options_t opts;
    memset(&opts, 0, sizeof(search_options_t));
    move_t moves[600];
    undo_t undos[30];
    memset(&last_pv, 0, sizeof(move_line_t));
    stop_search = false;

    ASSERT_EQ(CHECKMATE-5, search(&pos, &pv, 6, -CHECKMATE, CHECKMATE, moves, undos, 
        &stats, &opts));
    ASSERT_EQ(5, pv.n);
    ASSERT_EQ(to_move(ROOK, F6, A6), pv.mv[0]);
    ASSERT_EQ(to_move(PAWN, F7, F6), pv.mv[1]);
    ASSERT_EQ(to_capture(BISHOP, E5, F6, PAWN), pv.mv[2]);
    ASSERT_EQ(to_move(ROOK, G8, G7), pv.mv[3]);
    ASSERT_EQ(to_capture(ROOK, A6, A8, ROOK), pv.mv[4]);
}

TEST(search_test, stalemate)
{
    clear_hash_tables();
    position_t pos;
    set_pos(&pos, "8/6p1/5p2/5k1K/7P/8/8/8 w - -");
    move_line_t pv;
    stats_t stats;
    memset(&stats, 0, sizeof(stats_t));
    search_options_t opts;
    memset(&opts, 0, sizeof(search_options_t));
    move_t moves[100];
    undo_t undos[1];
    memset(&last_pv, 0, sizeof(move_line_t));
    stop_search = false;

    ASSERT_EQ(0, search(&pos, &pv, 1, -CHECKMATE, CHECKMATE, moves, undos, &stats, &opts));
    ASSERT_EQ(0, pv.n);
}


TEST(search_test, stop_search)
{
    clear_hash_tables();
    position_t pos;
    reset_pos(&pos);
    move_line_t pv;
    stats_t stats;
    memset(&stats, 0, sizeof(stats_t));
    search_options_t opts;
    memset(&opts, 0, sizeof(search_options_t));
    move_t moves[100];
    undo_t undos[3];
    memset(&last_pv, 0, sizeof(move_line_t));
    stop_search = true;

    // start a depth 3 search
    search(&pos, &pv, 3, -CHECKMATE, CHECKMATE, moves, undos, &stats, &opts);

    // visit one node per depth, down the left side of the tree
    ASSERT_EQ(3U, stats.nodes);
    ASSERT_EQ(1U, stats.qnodes);

    // the PV should not have been updated
    ASSERT_EQ(0, pv.n);
}


TEST(search_test, search_stops_on_time)
{
    clear_hash_tables();
    position_t pos;
    reset_pos(&pos);
    move_line_t pv;
    stats_t stats;
    memset(&stats, 0, sizeof(stats_t));
    search_options_t opts;
    memset(&opts, 0, sizeof(search_options_t));
    move_t moves[1000];
    undo_t undos[50];
    memset(&last_pv, 0, sizeof(move_line_t));
    stop_search = false;

    // start a depth 30 search, with a stop time in 500 ms
    opts.start_time = milli_timer();
    opts.stop_time = opts.start_time + 500;
    opts.nodes_between_time_checks = 1000UL;
    ASSERT_EQ(
        eval(&pos, false, false), 
        search(&pos, &pv, 30, -CHECKMATE, CHECKMATE, moves, undos, &stats, &opts));
    uint64_t stop_time = milli_timer();

    // the search should have stopped within 100 ms (which is very "generous")
    // from when we told it to.
    ASSERT_TRUE(stop_time - opts.stop_time < 100); 
}
