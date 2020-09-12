#include <prophet/movegen.h>
#include <prophet/search.h>

#include <gtest/gtest.h>

extern bool stop_search;

/**
 * More comprehensive iterator tests are in the chess4j project.  We do a simple
 * test here and verify equality to chess4j's.
 */

TEST(search_test, iterate_from_initial_pos)
{
    position_t pos;
    reset_pos(&pos);

    move_t moves[500];
    undo_t undos[50];

    iterator_options_t opts;
    memset(&opts, 0, sizeof(iterator_options_t));
    opts.early_exit_ok = false;
    opts.max_depth = 3;
    opts.post_mode = false;

    iterator_context_t ctx;
    ctx.pos = &pos;
    ctx.move_stack = moves;
    ctx.undo_stack = undos;

    move_line_t pv;
    stop_search = false;
    pv = iterate(&opts, &ctx);


    ASSERT_GE(pv.n, 3);

    for (int i=0; i<pv.n; i++) 
    {
        undo_t u;
        ASSERT_TRUE(is_legal_move(pv.mv[i], &pos));
        apply_move(&pos, pv.mv[i], &u);
    }
}

TEST(search_test, iterate_from_mating_position)
{
    position_t pos;

    // set up fool's mate
    reset_pos(&pos);
    undo_t u;
    apply_move(&pos, to_move(PAWN, F2, F3), &u);
    apply_move(&pos, to_move(PAWN, E7, E5), &u);
    apply_move(&pos, to_move(PAWN, G2, G4), &u);

    move_t moves[500];
    undo_t undos[50];

    iterator_options_t opts;
    memset(&opts, 0, sizeof(iterator_options_t));
    opts.early_exit_ok = false;
    opts.max_depth = 3;
    opts.post_mode = false;

    iterator_context_t ctx;
    ctx.pos = &pos;
    ctx.move_stack = moves;
    ctx.undo_stack = undos;

    move_line_t pv;
    stop_search = false;
    pv = iterate(&opts, &ctx);


    ASSERT_EQ(pv.n, 1);
    ASSERT_EQ(to_move(QUEEN, D8, H4), pv.mv[0]);
}


TEST(search_test, iterator_always_produces_move)
{
    position_t pos;
    reset_pos(&pos);

    move_t moves[500];
    undo_t undos[50];

    iterator_options_t opts;
    memset(&opts, 0, sizeof(iterator_options_t));
    opts.early_exit_ok = false;
    opts.max_depth = 3;
    opts.post_mode = false;

    iterator_context_t ctx;
    ctx.pos = &pos;
    ctx.move_stack = moves;
    ctx.undo_stack = undos;

    move_line_t pv;
    stop_search = true;
    pv = iterate(&opts, &ctx);

    ASSERT_EQ(pv.n, 1);
    ASSERT_TRUE(is_legal_move(pv.mv[0], &pos));
}
