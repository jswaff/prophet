#include "position/position_internal.h"

#include "prophet/move.h"

#include <gtest/gtest.h>

#include <string.h>

TEST(pos_test, draw_rep)
{
    position_t pos;
    reset_pos(&pos);

    undo_t undos[11];

    EXPECT_FALSE(is_draw_rep(&pos, undos, 2));

    apply_move(&pos, to_move(PAWN, E2, E4), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos, 2));

    apply_move(&pos, to_move(KNIGHT, G8, F6), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos, 2));

    apply_move(&pos, to_move(KNIGHT, G1, F3), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos, 2));

    apply_move(&pos, to_move(KNIGHT, F6, G8), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos, 2));

    // still 1 (looks like 1. e2e4 but no ep square)
    apply_move(&pos, to_move(KNIGHT, F3, G1), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos, 2));

    apply_move(&pos, to_move(KNIGHT, G8, F6), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos, 2)); // 2

    apply_move(&pos, to_move(KNIGHT, G1, F3), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos, 2)); // 2

    apply_move(&pos, to_move(KNIGHT, F6, G8), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos, 2)); //2

    apply_move(&pos, to_move(KNIGHT, F3, G1), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos, 2)); // 2

    apply_move(&pos, to_move(KNIGHT, G8, F6), undos + pos.move_counter);
    EXPECT_TRUE(is_draw_rep(&pos, undos, 2)); // 3

    apply_move(&pos, to_move(PAWN, D2, D4), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos, 2));

}


TEST(pos_test, draw_rep_fen)
{
    position_t pos;
    undo_t undos[100];
    memset(undos, 0, sizeof(undos));

    ASSERT_TRUE(
        set_pos(&pos,"7k/7p/8/8/8/8/7P/7K w - - 12 47"));

    EXPECT_EQ(pos.move_counter, 92U);
    EXPECT_FALSE(is_draw_rep(&pos, undos, 2));

    apply_move(&pos, to_move(KING, H8, G8), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos, 2));

    apply_move(&pos, to_move(KING, H1, G1), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos, 2));

    apply_move(&pos, to_move(KING, G8, H8), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos, 2));

    apply_move(&pos, to_move(KING, G1, H1), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos, 2));

    apply_move(&pos, to_move(KING, H8, G8), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos, 2));

    apply_move(&pos, to_move(KING, H1, G1), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos, 2));

    apply_move(&pos, to_move(KING, G8, H8), undos + pos.move_counter);
    EXPECT_FALSE(is_draw_rep(&pos, undos, 2));

    apply_move(&pos, to_move(KING, G1, H1), undos + pos.move_counter);
    EXPECT_TRUE(is_draw_rep(&pos, undos, 2));

    EXPECT_EQ(100U, pos.move_counter);
    EXPECT_EQ(20U, pos.fifty_counter);
}
