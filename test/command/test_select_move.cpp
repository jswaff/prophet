#include <gtest/gtest.h>

#include <prophet/const.h>
#include <prophet/movegen.h>
#include <prophet/position/position.h>

#include "../../src/command/command_internal.h"

TEST(command_test, select_move)
{
    position pos;
    reset_pos(&pos);

    move_t mv = select_move(&pos);
    EXPECT_NE(NO_MOVE, mv);
    EXPECT_TRUE(is_legal_move(mv, &pos));


    // a checkmated position
    set_pos(&pos, 
        "3b1q1q/1N2PRQ1/rR3KBr/B4PP1/2Pk1r1b/1P2P1N1/2P2P2/8 b - -");
    EXPECT_EQ(NO_MOVE, select_move(&pos));

    // a stalemate position
    set_pos(&pos, "7k/5K2/6Q1/8/8/8/8/8 b - -");
    EXPECT_EQ(NO_MOVE, select_move(&pos));
}
