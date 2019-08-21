#include <gtest/gtest.h>

#include <prophet/position/move.h>
#include <prophet/position/position.h>
#include <prophet/util/legal.h>

/**
 * Test that the test utility function to measure hamming distances is correct.
 */
TEST(legal_test, is_legal_move_test)
{
    position pos;
    reset_pos(&pos);

    move legal = to_move(PAWN, E2, E4);
    EXPECT_TRUE(is_legal_move(legal, &pos));

    move illegal = to_move(PAWN, E2, E5);
    EXPECT_FALSE(is_legal_move(illegal, &pos));
}
