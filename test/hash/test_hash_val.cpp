#include "hash/hash_internal.h"

#include "prophet/const.h"
#include "prophet/move.h"

#include <gtest/gtest.h>

#include <stdint.h>

TEST(hash_test, hash_val_upper_bound)
{
    int32_t depth = 45;
    int32_t score = 122;
    move_t mv = to_move(PAWN, E2, E4);
    uint32_t hash_age = 17;
    uint64_t val = build_hash_val(UPPER_BOUND, depth, score, mv, hash_age);

    ASSERT_EQ(UPPER_BOUND, get_hash_entry_type(val));
    ASSERT_EQ(depth, get_hash_entry_depth(val));
    ASSERT_EQ(score, get_hash_entry_score(val));
    ASSERT_EQ(to_move(PAWN, E2, E4), get_hash_entry_move(val));
    ASSERT_EQ(hash_age, get_hash_entry_age(val));
}


TEST(hash_test, hash_val_lower_bound)
{
    int32_t depth = 14;
    int32_t score = -109;
    move_t mv = to_move(KNIGHT, B1, A3);
    uint32_t hash_age = 21;
    uint64_t val = build_hash_val(LOWER_BOUND, depth, score, mv, hash_age);

    ASSERT_EQ(LOWER_BOUND, get_hash_entry_type(val));
    ASSERT_EQ(depth, get_hash_entry_depth(val));
    ASSERT_EQ(score, get_hash_entry_score(val));
    ASSERT_EQ(mv, get_hash_entry_move(val));
    ASSERT_EQ(hash_age, get_hash_entry_age(val));
}


TEST(hash_test, hash_val_exact_score)
{
    int32_t depth = 1;
    int32_t score = -54;
    move_t mv = to_capture(PAWN, E7, E8, BISHOP);
    set_promopiece(&mv, QUEEN);
    uint32_t hash_age = 46;
    uint64_t val = build_hash_val(EXACT_SCORE, depth, score, mv, hash_age);

    ASSERT_EQ(EXACT_SCORE, get_hash_entry_type(val));
    ASSERT_EQ(depth, get_hash_entry_depth(val));
    ASSERT_EQ(score, get_hash_entry_score(val));
    ASSERT_EQ(mv, get_hash_entry_move(val));
    ASSERT_EQ(hash_age, get_hash_entry_age(val));
}


TEST(hash_test, lower_bound_mate)
{
    int32_t depth = 17;
    int32_t score = CHECKMATE - 9;
    move_t mv = to_move(QUEEN, A1, A8);
    uint32_t hash_age = 99;
    uint64_t val = build_hash_val(LOWER_BOUND, depth, score, mv, hash_age);

    ASSERT_EQ(LOWER_BOUND, get_hash_entry_type(val));
    ASSERT_EQ(depth, get_hash_entry_depth(val));
    ASSERT_EQ(CHECKMATE - 500, get_hash_entry_score(val));
    ASSERT_EQ(mv, get_hash_entry_move(val));    
    ASSERT_EQ(hash_age, get_hash_entry_age(val));
}


TEST(hash_test, transform_upper_bound_mate)
{
    int32_t depth = 7;
    int32_t score = CHECKMATE - 3;
    move_t mv = to_move(ROOK, E7, E8);
    uint32_t hash_age = 10;
    uint64_t val = build_hash_val(UPPER_BOUND, depth, score, mv, hash_age);

    ASSERT_EQ(MOVE_ONLY, get_hash_entry_type(val));
    ASSERT_EQ(mv, get_hash_entry_move(val));
    ASSERT_EQ(hash_age, get_hash_entry_age(val));
}


TEST(hash_test, transform_exact_score_mate)
{
    int32_t depth = 7;
    int32_t score = CHECKMATE - 3;
    move_t mv = to_move(ROOK, E7, E8);
    uint32_t hash_age = 0;
    uint64_t val = build_hash_val(EXACT_SCORE, depth, score, mv, hash_age);

    ASSERT_EQ(LOWER_BOUND, get_hash_entry_type(val));
    ASSERT_EQ(depth, get_hash_entry_depth(val));
    ASSERT_EQ(CHECKMATE - 500, get_hash_entry_score(val));
    ASSERT_EQ(mv, get_hash_entry_move(val));
    ASSERT_EQ(hash_age, get_hash_entry_age(val));
}


TEST(hash_test, transform_lower_bound_mated)
{
    int32_t depth = 17;
    int32_t score = -CHECKMATE + 10;
    move_t mv = to_move(QUEEN, A8, A1);
    uint32_t hash_age = 255;
    uint64_t val = build_hash_val(LOWER_BOUND, depth, score, mv, hash_age);

    ASSERT_EQ(MOVE_ONLY, get_hash_entry_type(val));
    ASSERT_EQ(mv, get_hash_entry_move(val));    
    ASSERT_EQ(hash_age, get_hash_entry_age(val));
}


TEST(hash_test, upper_bound_mated)
{
    int32_t depth = 7;
    int32_t score = -CHECKMATE + 8;
    move_t mv = to_move(ROOK, E7, E8);
    uint32_t hash_age = 23;
    uint64_t val = build_hash_val(UPPER_BOUND, depth, score, mv, hash_age);

    ASSERT_EQ(UPPER_BOUND, get_hash_entry_type(val));
    ASSERT_EQ(depth, get_hash_entry_depth(val));
    ASSERT_EQ(-CHECKMATE + 500, get_hash_entry_score(val));
    ASSERT_EQ(mv, get_hash_entry_move(val));
    ASSERT_EQ(hash_age, get_hash_entry_age(val));
}


TEST(hash_test, transform_exact_score_mated)
{
    int32_t depth = 7;
    int32_t score = -CHECKMATE + 4;
    move_t mv = to_move(ROOK, E2, E1);
    uint32_t hash_age = 25;
    uint64_t val = build_hash_val(EXACT_SCORE, depth, score, mv, hash_age);

    ASSERT_EQ(UPPER_BOUND, get_hash_entry_type(val));
    ASSERT_EQ(depth, get_hash_entry_depth(val));
    ASSERT_EQ(-CHECKMATE + 500, get_hash_entry_score(val));
    ASSERT_EQ(mv, get_hash_entry_move(val));
    ASSERT_EQ(hash_age, get_hash_entry_age(val));
}


TEST(hash_test, empty_val_is_NO_MOVE)
{
    uint64_t val = 0;
    ASSERT_EQ(NO_MOVE, get_hash_entry_move(val));
}


TEST(hash_test, pawn_score)
{
    int32_t mg_score = -33;
    int32_t eg_score = 17;

    uint64_t val = build_pawn_hash_val(mg_score, eg_score);

    ASSERT_EQ(mg_score, get_pawn_hash_entry_mg_score(val));
    ASSERT_EQ(eg_score, get_pawn_hash_entry_eg_score(val));
}
