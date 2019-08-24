#include <gtest/gtest.h>

#include <prophet/position/piece.h>

#include "../../src/position/position_internal.h"

TEST(pos_test, build_hash_key)
{
    position pos;
    reset_pos(&pos);

    uint64_t orig_key = build_hash_key(&pos);
    EXPECT_NE(orig_key, 0U);

    // removing white piece changes key
    remove_piece(&pos, A1);
    uint64_t hkey = build_hash_key(&pos);
    EXPECT_NE(hkey, orig_key);

    // adding piece back restores key
    add_piece(&pos, ROOK, A1);
    EXPECT_EQ(build_hash_key(&pos), orig_key);

    // removing black piece changes key
    remove_piece(&pos, A7);
    hkey = build_hash_key(&pos);
    EXPECT_NE(hkey, orig_key);

    // adding piece back restores key
    add_piece(&pos, -PAWN, A7);
    EXPECT_EQ(build_hash_key(&pos), orig_key);

    // changing player changes key
    pos.player = BLACK;
    hkey = build_hash_key(&pos);
    EXPECT_NE(hkey, orig_key);

    // switching the player back restores key
    pos.player = WHITE;
    EXPECT_EQ(build_hash_key(&pos), orig_key);

    // castling rights changes key
    pos.castling_rights = CASTLE_NOT_BQ;
    hkey = build_hash_key(&pos);
    EXPECT_NE(hkey, orig_key);

    // switching the player back restores key
    pos.castling_rights = CASTLE_ALL;
    EXPECT_EQ(build_hash_key(&pos), orig_key);
}

TEST(pos_test, build_hash_key_ep)
{
    position pos;
    reset_pos(&pos);

    uint64_t orig_key = build_hash_key(&pos);

    // move to E4
    undo my_undo;
    apply_move(&pos, to_move(PAWN, E2, E4), &my_undo);
    uint64_t key1 = build_hash_key(&pos);
    EXPECT_NE(key1, orig_key);

    // clearing the EP square should change the hash key
    pos.ep_sq = NO_SQUARE;
    EXPECT_NE(build_hash_key(&pos), key1);

    // reset the EP square
    pos.ep_sq = E3;
    EXPECT_EQ(build_hash_key(&pos), key1);

    // undo the move, back to the starting position
    undo_move(&pos, &my_undo);
    EXPECT_EQ(build_hash_key(&pos), orig_key);
}

TEST(pos_test, build_pawn_key)
{
    position pos;
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
