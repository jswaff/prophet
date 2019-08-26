#include <prophet/position/position.h>

#include <gtest/gtest.h>


TEST(pos_test, draw_rep)
{
    position_t pos;
    reset_pos(&pos);

    undo_t undos[11];

    EXPECT_FALSE(is_draw_rep(&pos, undos));

    apply_move(&pos, to_move(PAWN, E2, E4), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos));

    apply_move(&pos, to_move(KNIGHT, G8, F6), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos));

    apply_move(&pos, to_move(KNIGHT, G1, F3), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos));

    apply_move(&pos, to_move(KNIGHT, F6, G8), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos));

    // still 1 (looks like 1. e2e4 but no ep square)
    apply_move(&pos, to_move(KNIGHT, F3, G1), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos));

    apply_move(&pos, to_move(KNIGHT, G8, F6), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos)); // 2

    apply_move(&pos, to_move(KNIGHT, G1, F3), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos)); // 2

    apply_move(&pos, to_move(KNIGHT, F6, G8), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos)); //2

    apply_move(&pos, to_move(KNIGHT, F3, G1), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos)); // 2

    apply_move(&pos, to_move(KNIGHT, G8, F6), undos + pos.move_counter);
    EXPECT_TRUE(is_draw_rep(&pos, undos)); // 3

    apply_move(&pos, to_move(PAWN, D2, D4), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos));

}
