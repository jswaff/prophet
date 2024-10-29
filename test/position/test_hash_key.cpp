#include "../../src/position/position_internal.h"

#include "prophet/move.h"
#include "prophet/position.h"

#include <gtest/gtest.h>

#include <stdint.h>

TEST(pos_test, build_hash_key)
{
    position_t pos;
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
    position_t pos;
    reset_pos(&pos);

    uint64_t orig_key = build_hash_key(&pos);

    // move to E4
    undo_t my_undo;
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


/**
 * test that hash keys are equal after transposition.
 */
TEST(pos_test, hash_key_transposition)
{
    position_t pos;
    reset_pos(&pos);

    position_t pos2;
    reset_pos(&pos2);

    undo_t u;

    ASSERT_EQ(pos.hash_key, pos2.hash_key);

    // step through the Queen's Gambit
    apply_move(&pos, to_move(PAWN, D2, D4), &u);
    apply_move(&pos, to_move(PAWN, D7, D5), &u);
    apply_move(&pos, to_move(PAWN, C2, C4), &u);
    apply_move(&pos, to_move(PAWN, E7, E6), &u);
    apply_move(&pos, to_move(KNIGHT, B1, C3), &u);
    apply_move(&pos, to_move(KNIGHT, G8, F6), &u);

    // step through the English Opening
    apply_move(&pos2, to_move(PAWN, C2, C4), &u);
    apply_move(&pos2, to_move(KNIGHT, G8, F6), &u);
    apply_move(&pos2, to_move(KNIGHT, B1, C3), &u);
    apply_move(&pos2, to_move(PAWN, E7, E6), &u);
    apply_move(&pos2, to_move(PAWN, D2, D4), &u);
    apply_move(&pos2, to_move(PAWN, D7, D5), &u);

    // the positions would be equal at this point, except for fifty counter
    // and eq square
    ASSERT_FALSE(pos_equals(&pos, &pos2, true));
    ASSERT_NE(pos.hash_key, pos2.hash_key);

    // by adding a pawn move the positions are equal
    apply_move(&pos, to_move(PAWN, G2, G3), &u);
    apply_move(&pos2, to_move(PAWN, G2, G3), &u);
    ASSERT_TRUE(pos_equals(&pos, &pos2, true));
    ASSERT_EQ(pos.hash_key, pos2.hash_key);
}


TEST(pos_test, hash_key_transposition2)
{
    position_t pos;
    reset_pos(&pos);

    position_t pos2;
    reset_pos(&pos2);

    undo_t u;

    ASSERT_EQ(pos.hash_key, pos2.hash_key);

    // step through the French Defense
    apply_move(&pos, to_move(PAWN, E2, E4), &u);
    apply_move(&pos, to_move(PAWN, E7, E6), &u);
    apply_move(&pos, to_move(PAWN, D2, D4), &u);
    apply_move(&pos, to_move(PAWN, D7, D5), &u);
    apply_move(&pos, to_capture(PAWN, E4, D5, PAWN), &u);
    apply_move(&pos, to_capture(PAWN, E6, D5, PAWN), &u);
    apply_move(&pos, to_move(KNIGHT, G1, F3), &u);
    apply_move(&pos, to_move(KNIGHT, G8, F6), &u);

    // step through the Petrov Defense
    apply_move(&pos2, to_move(PAWN, E2, E4), &u);
    apply_move(&pos2, to_move(PAWN, E7, E5), &u);
    apply_move(&pos2, to_move(KNIGHT, G1, F3), &u);
    apply_move(&pos2, to_move(KNIGHT, G8, F6), &u);
    apply_move(&pos2, to_capture(KNIGHT, F3, E5, PAWN), &u);
    apply_move(&pos2, to_move(PAWN, D7, D6), &u);
    apply_move(&pos2, to_move(KNIGHT, E5, F3), &u);
    apply_move(&pos2, to_capture(KNIGHT, F6, E4, PAWN), &u);
    apply_move(&pos2, to_move(PAWN, D2, D3), &u);
    apply_move(&pos2, to_move(KNIGHT, E4, F6), &u);
    apply_move(&pos2, to_move(PAWN, D3, D4), &u);
    apply_move(&pos2, to_move(PAWN, D6, D5), &u);

    // the positions would be equal at this point, except for fifty counter
    // and move counter which are not factored into the key
    ASSERT_FALSE(pos_equals(&pos, &pos2, true));
    ASSERT_EQ(pos.hash_key, pos2.hash_key);
}
