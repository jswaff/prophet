#include <gtest/gtest.h>

#include "../../src/position/position_internal.h"


TEST(pos_test, build_pawn_key)
{
    position_t pos;
    reset_pos(&pos);

    uint64_t orig_key = build_pawn_key(&pos);

    EXPECT_NE(orig_key, 0U);

    // removing white pawn changes key
    remove_piece(&pos, A2);
    uint64_t hkey = build_pawn_key(&pos);
    EXPECT_NE(hkey, orig_key);

    // removing white rook does not change key
    remove_piece(&pos, A1);
    EXPECT_EQ(build_pawn_key(&pos), hkey);

    // replacing white rook does not change key
    add_piece(&pos, ROOK, A1);
    EXPECT_EQ(build_pawn_key(&pos), hkey);

    // replacing pawn restores key
    add_piece(&pos, PAWN, A2);
    EXPECT_EQ(build_pawn_key(&pos), orig_key);

    // removing black pawn changes key
    remove_piece(&pos, G7);
    hkey = build_pawn_key(&pos);
    EXPECT_NE(hkey, orig_key);

    // removing black bishop does not change key
    remove_piece(&pos, B8);
    EXPECT_EQ(build_pawn_key(&pos), hkey);

    // replacing black bishop does not change key
    add_piece(&pos, -BISHOP, B8);
    EXPECT_EQ(build_pawn_key(&pos), hkey);

    // replacing pawn restores key
    add_piece(&pos, -PAWN, G7);
    EXPECT_EQ(build_pawn_key(&pos), orig_key);

    // changing player does not change key
    pos.player = BLACK;
    EXPECT_EQ(build_pawn_key(&pos), orig_key);
}
