#include <prophet/hash.h>

#include <gtest/gtest.h>

TEST(hash_test, hash_val_upper_bound)
{
    int32_t depth = 45;
    move_t mv = to_move(PAWN, E2, E4);
    set_move_score(&mv, 123);
    uint64_t val = build_hash_val(UPPER_BOUND, depth, -CHECKMATE, mv);

    ASSERT_EQ(UPPER_BOUND, get_hash_entry_type(val));
    ASSERT_EQ(depth, get_hash_entry_depth(val));
    ASSERT_EQ(-CHECKMATE+500, get_hash_entry_score(val));
    ASSERT_EQ(to_move(PAWN, E2, E4), get_hash_entry_move(val));
}


TEST(hash_test, hash_val_lower_bound)
{
    int32_t depth = 14;
    int32_t score = -109;
    move_t mv = to_move(KNIGHT, B1, A3);
    uint64_t val = build_hash_val(LOWER_BOUND, depth, score, mv);

    ASSERT_EQ(LOWER_BOUND, get_hash_entry_type(val));
    ASSERT_EQ(depth, get_hash_entry_depth(val));
    ASSERT_EQ(score, get_hash_entry_score(val));
    ASSERT_EQ(mv, get_hash_entry_move(val));
}


TEST(hash_test, hash_val_exact_score)
{
    int32_t depth = 1;
    int32_t score = -54;
    move_t mv = to_capture(PAWN, E7, E8, BISHOP);
    set_promopiece(&mv, QUEEN);
    uint64_t val = build_hash_val(EXACT_SCORE, depth, score, mv);

    ASSERT_EQ(EXACT_SCORE, get_hash_entry_type(val));
    ASSERT_EQ(depth, get_hash_entry_depth(val));
    ASSERT_EQ(score, get_hash_entry_score(val));
    ASSERT_EQ(mv, get_hash_entry_move(val));
}
