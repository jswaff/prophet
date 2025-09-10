#include "search/search_internal.h"

#include "position/position_internal.h"

#include <gtest/gtest.h>

TEST(search_test, zugzwang)
{
    position_t pos;
    reset_pos(&pos);
    ASSERT_FALSE(zugzwang(&pos));

    // just kings
    set_pos(&pos,"7k/8/8/8/8/8/8/7K w - - ");
    ASSERT_TRUE(zugzwang(&pos));

    // single white bishop
    set_pos(&pos,"7k/8/8/B7/8/8/8/7K b - - ");
    ASSERT_TRUE(zugzwang(&pos));

    // white pawn black knight
    set_pos(&pos,"7k/8/8/n7/P7/8/8/7K b - - ");
    ASSERT_TRUE(zugzwang(&pos));

    // white rook black knight
    set_pos(&pos,"7k/8/8/n7/R7/8/8/7K b - - ");
    ASSERT_FALSE(zugzwang(&pos));
}