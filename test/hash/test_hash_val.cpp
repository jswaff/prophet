#include <prophet/hash.h>

#include <gtest/gtest.h>

TEST(hash_test, hash_val_upper_bound)
{
    int32_t depth = 45;
    move_t mv = to_move(PAWN, E2, E4); // TODO: with score
    uint64_t val = build_hash_val(UPPER_BOUND, depth, -CHECKMATE, mv);

    ASSERT_EQ(UPPER_BOUND, get_hash_entry_type(val));
    ASSERT_EQ(depth, get_hash_entry_depth(val));
    ASSERT_EQ(-CHECKMATE+500, get_hash_entry_score(val));
    ASSERT_EQ(mv, get_hash_entry_move(val));
}
