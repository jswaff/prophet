#include <prophet/movegen.h>
#include <prophet/search.h>

#include <gtest/gtest.h>

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
    opts.early_exit_ok = false;
    opts.max_depth = 3;
    opts.post_mode = false;

    iterator_context_t ctx;
    ctx.pos = &pos;
    ctx.move_stack = moves;
    ctx.undo_stack = undos;

    move_line_t pv;
    pv = iterate(&opts, &ctx);


    ASSERT_GE(pv.n, 3);

    for (int i=0; i<pv.n; i++) 
    {
        undo_t u;
        ASSERT_TRUE(is_legal_move(pv.mv[i], &pos));
        apply_move(&pos, pv.mv[i], &u);
    }
}
