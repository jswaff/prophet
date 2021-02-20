#include <prophet/bitmap.h>

#include <gtest/gtest.h>

#include "../../src/movegen/movegen_internal.h"


TEST(movegen_test, attackers_knight_rook)
{
    position_t pos;
    set_pos(&pos,"1k1r3q/1ppn3p/p4b2/4p3/8/P2N2P1/1PP1R1BP/2K1Q3 w - -");

    uint64_t attackers_map = attackers(&pos, E5, WHITE);

    ASSERT_TRUE(attackers_map & square_to_bitmap(D3));
    ASSERT_TRUE(attackers_map & square_to_bitmap(E2));
    ASSERT_EQ(2U, popcnt(attackers_map));
}


TEST(movegen_test, attackers_knight_bishop)
{
    position_t pos;
    set_pos(&pos,"1k1r3q/1ppn3p/p4b2/4p3/8/P2N2P1/1PP1R1BP/2K1Q3 w - -");

    uint64_t attackers_map = attackers(&pos, E5, BLACK);

    ASSERT_TRUE(attackers_map & square_to_bitmap(D7));
    ASSERT_TRUE(attackers_map & square_to_bitmap(F6));
    ASSERT_EQ(2U, popcnt(attackers_map));
}


TEST(movegen_test, attackers_queen)
{
    position_t pos;
    set_pos(&pos,"8/p1k5/1p6/8/3b4/1Q6/8/7K w - -");

    uint64_t attackers_map = attackers(&pos, B6, WHITE);

    ASSERT_TRUE(attackers_map & square_to_bitmap(B3));
    ASSERT_EQ(1U, popcnt(attackers_map));
}


TEST(movegen_test, attackers_pawn_king_bishop)
{
    position_t pos;
    set_pos(&pos,"8/p1k5/1p6/8/3b4/1Q6/8/7K w - -");

    uint64_t attackers_map = attackers(&pos, B6, BLACK);

    ASSERT_TRUE(attackers_map & square_to_bitmap(A7));
    ASSERT_TRUE(attackers_map & square_to_bitmap(C7));
    ASSERT_TRUE(attackers_map & square_to_bitmap(D4));
    ASSERT_EQ(3U, popcnt(attackers_map));
}
