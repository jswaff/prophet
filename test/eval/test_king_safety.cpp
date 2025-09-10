#include "eval/eval_internal.h"

#include "position/position_internal.h"

#include <gtest/gtest.h>


TEST(eval_test, king_safety_middle_files)
{
    position_t pos;

    // initial position then e3... no penalty
    ASSERT_TRUE(set_pos(&pos, 
        "rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR w KQkq - 0 1"));

    EXPECT_EQ(0, eval_king_safety(&pos, true));
    EXPECT_EQ(0, eval_king_safety(&pos, false));

    // open file for both
    ASSERT_TRUE(set_pos(&pos,
        "rnbqkbnr/pppp1ppp/8/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1"));

    EXPECT_EQ(king_safety_middle_open_file, 
        eval_king_safety(&pos, true));
    EXPECT_EQ(king_safety_middle_open_file, 
        eval_king_safety(&pos, false));

    // remove both queens.  open e file.  put black king on d8
    // white should be penalized but black is not.
    ASSERT_TRUE(set_pos(&pos,
        "rnbk1bnr/pppp1ppp/8/8/8/8/PPPP1PPP/RNB1KBNR b KQ - 0 1"));

    EXPECT_EQ(king_safety_middle_open_file, 
        eval_king_safety(&pos, true));
    EXPECT_EQ(0, eval_king_safety(&pos, false));

}

TEST(eval_test, king_safety_king_side)
{
    position_t pos;

    // no pawns advanced
    ASSERT_TRUE(set_pos(&pos, 
        "rnbq1rk1/pppppppp/8/8/8/8/PPPPPPPP/RNBQ1RK1 w - - 0 1"));

    EXPECT_EQ(0, eval_king_safety(&pos, true));
    EXPECT_EQ(0, eval_king_safety(&pos, false));

    // white pawn on F3
    ASSERT_TRUE(set_pos(&pos, 
        "rnbq1rk1/pppppppp/8/8/8/5P2/PPPPP1PP/RNBQ1RK1 w - - 0 1"));

    EXPECT_EQ(king_safety_pawn_one_away, 
        eval_king_safety(&pos, true));
    EXPECT_EQ(0, eval_king_safety(&pos, false));

    // white pawn on G4
    ASSERT_TRUE(set_pos(&pos, 
        "rnbq1rk1/pppppppp/8/8/6P1/8/PPPPPP1P/RNBQ1RK1 w - - 0 1"));

    EXPECT_EQ(king_safety_pawn_two_away, 
        eval_king_safety(&pos, true));
    EXPECT_EQ(0, eval_king_safety(&pos, false));
}

TEST(eval_test, king_safety_queen_side)
{
    position_t pos;

    // pawn on C3
    ASSERT_TRUE(set_pos(&pos, 
        "1krq1bnr/pppppppp/8/8/8/2P5/PP1PPPPP/1KRQ1BNR w - - 0 1"));

    EXPECT_EQ(king_safety_pawn_one_away, 
        eval_king_safety(&pos, true));
    EXPECT_EQ(0, eval_king_safety(&pos, false));

    // white pawn on B4
    ASSERT_TRUE(set_pos(&pos, 
        "1krq1bnr/pppppppp/8/8/1P6/8/P1PPPPPP/1KRQ1BNR w - - 0 1"));

    EXPECT_EQ(king_safety_pawn_two_away, 
        eval_king_safety(&pos, true));
    EXPECT_EQ(0, eval_king_safety(&pos, false));

    // black pawn on A4
    ASSERT_TRUE(set_pos(&pos, 
        "1krq1bnr/1ppppppp/8/8/p7/8/PPPPPPPP/1KRQ1BNR b - - 0 1"));

    EXPECT_EQ(0, eval_king_safety(&pos, true));
    EXPECT_EQ(king_safety_wing_pawn_far_away, 
        eval_king_safety(&pos, false));
}
