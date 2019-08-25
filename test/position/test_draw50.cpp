#include <prophet/position/position.h>

#include <gtest/gtest.h>


TEST(pos_test, draw_50)
{
    position_t pos;
    reset_pos(&pos);

    EXPECT_FALSE(is_draw50(&pos));

    // move knights out and back in 25 times.  that will take the 50 move
    // counter to 25 x 4 = 100.  Only on the last move should the draw be
    // claimed.
    undo_t u;
    for (int i=0; i<25; i++)
    {
        apply_move(&pos, to_move(KNIGHT, G1, F3), &u);
        EXPECT_FALSE(is_draw50(&pos));

        apply_move(&pos, to_move(KNIGHT, G8, F6), &u);
        EXPECT_FALSE(is_draw50(&pos));
        apply_move(&pos, to_move(KNIGHT, F3, G1), &u);

        EXPECT_FALSE(is_draw50(&pos));

        apply_move(&pos, to_move(KNIGHT, F6, G8), &u);
        EXPECT_EQ(i==24, is_draw50(&pos));
    }

    EXPECT_EQ(100U, pos.fifty_counter);

    // move a pawn and it's reset
    apply_move(&pos, to_move(PAWN, E2, E3), &u);
    EXPECT_EQ(0U, pos.fifty_counter);
    EXPECT_FALSE(is_draw50(&pos));    
}
