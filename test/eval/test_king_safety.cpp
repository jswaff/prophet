#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

TEST(eval_test, DISABLED_king_safety_middle_files)
{
    position_t pos;

    // initial position then e3... no penalty
    ASSERT_TRUE(set_pos(&pos, 
        "rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR w KQkq - 0 1"));

    EXPECT_EQ(0, eval_king_safety(&pos, true));
    EXPECT_EQ(0, eval_king_safety(&pos, false));

    // open file for both, so still no penalty
    ASSERT_TRUE(set_pos(&pos,
        "rnbqkbnr/pppp1ppp/8/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1"));

    EXPECT_EQ(0, eval_king_safety(&pos, true));
    EXPECT_EQ(0, eval_king_safety(&pos, false));

    // remove both queens.  open e file.  put black king on d8
    ASSERT_TRUE(set_pos(&pos,
        "rnbk1bnr/pppp1ppp/8/8/8/8/PPPP1PPP/RNB1KBNR b KQ - 0 1"));

    EXPECT_EQ(0, eval_king_safety(&pos, true));
    EXPECT_EQ(king_safety_middle_open_file, 
        eval_king_safety(&pos, false));

}

