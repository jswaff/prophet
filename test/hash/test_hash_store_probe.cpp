#include <prophet/hash.h>

#include <gtest/gtest.h>

TEST(hash_test, store_and_probe)
{
    hash_table_t hash_table;

    // define a small table
    hash_entry_t tbl[10];
    hash_table.capacity = 10;
    hash_table.tbl = tbl;
    clear_hash_table(&hash_table);
    EXPECT_EQ(0U, hash_table.probes);
    EXPECT_EQ(0U, hash_table.hits);


    // store using key < capacity
    uint64_t val = 0xFFU;
    ASSERT_EQ(0U, probe_hash(&hash_table, 5));
    EXPECT_EQ(1U, hash_table.probes);
    EXPECT_EQ(0U, hash_table.hits);

    store_hash_entry(&hash_table, 5, val);
    ASSERT_EQ(val, probe_hash(&hash_table, 5));
    EXPECT_EQ(2U, hash_table.probes);
    EXPECT_EQ(1U, hash_table.hits);


    // using key > capacity
    ASSERT_EQ(0U, probe_hash(&hash_table, 12));
    EXPECT_EQ(3U, hash_table.probes);
    EXPECT_EQ(1U, hash_table.hits);

    store_hash_entry(&hash_table, 12, val);
    ASSERT_EQ(val, probe_hash(&hash_table, 12));
    EXPECT_EQ(4U, hash_table.probes);
    EXPECT_EQ(2U, hash_table.hits);

    // key collision - this would pull from the same bucket, 
    // but the key doesn't match. 
    ASSERT_EQ(0U, probe_hash(&hash_table, 2));
    EXPECT_EQ(5U, hash_table.probes);
    EXPECT_EQ(2U, hash_table.hits);

    // now store in the bucket we just probed
    store_hash_entry(&hash_table, 2, val);
    ASSERT_EQ(val, probe_hash(&hash_table, 2));
    EXPECT_EQ(6U, hash_table.probes);
    EXPECT_EQ(3U, hash_table.hits);

    // the previous entry should still be there as there are multiple slots
    ASSERT_EQ(val, probe_hash(&hash_table, 12));
    EXPECT_EQ(7U, hash_table.probes);
    EXPECT_EQ(4U, hash_table.hits);
}

TEST(hash_test, replacement_strategy)
{
    hash_table_t hash_table;

    // define a small table
    hash_entry_t tbl[10];
    hash_table.capacity = 10;
    hash_table.tbl = tbl;
    clear_hash_table(&hash_table);

    uint64_t key = 5;
    ASSERT_EQ(0U, probe_hash(&hash_table, key));

    // store a value
    move_t mv = to_move(PAWN, E2, E4);
    uint64_t val = build_hash_val(LOWER_BOUND, 7, 100, mv, 1); 
    store_hash_entry(&hash_table, key, val);
    ASSERT_EQ(val, probe_hash(&hash_table, key));

    // store another value using the same key, with the same age and
    // a greater depth.  Because it uses the same key, it should 
    // overwrite the previous entry.
    uint64_t key2 = key;
    move_t mv2 = to_move(PAWN, D2, D3);
    uint64_t val2 = build_hash_val(LOWER_BOUND, 9, 99, mv2, 1);
    store_hash_entry(&hash_table, key2, val2);
    ASSERT_EQ(val2, probe_hash(&hash_table, key2));


    // store another value with a different key, and a greater depth
    // than the previous entry, and the same age. 
    uint64_t key3 = 3;
    move_t mv3 = to_move(KNIGHT, G1, F3);
    uint64_t val3 = build_hash_val(UPPER_BOUND, 10, -76, mv3, 1);
    store_hash_entry(&hash_table, key3, val3);
    ASSERT_EQ(val3, probe_hash(&hash_table, key3));


    // store yet another value with a different key that maps to the
    // same bucket, a shallower depth, and the same age.  It should 
    // not overwrite the previous entry, but be written to the next
    // slot.
    uint64_t key4 = 13;
    uint64_t val4 = build_hash_val(UPPER_BOUND, 5, 101, NO_MOVE, 1);
    store_hash_entry(&hash_table, key4, val4);
    ASSERT_EQ(val4, probe_hash(&hash_table, key4));
    ASSERT_EQ(val3, probe_hash(&hash_table, key3));

}


TEST(hash_test, replacement_strategy2)
{
    hash_table_t hash_table;

    // define a small table
    hash_entry_t tbl[10];
    hash_table.capacity = 10;
    hash_table.tbl = tbl;
    clear_hash_table(&hash_table);

    // first ensure all slots for a key are empty.
    // then store values to the same bucket, with different keys to fill up
    // each slot.  Each entry should have a smaller depth but the same hash age.
    // Once the slots are full, write one more entry with a smaller search.  It
    // should overwrite the smallest depth from before.  Finally, write one more
    // entry with a larger hash age.  It should overwrite the first slot.

    uint64_t k = 4;
    ASSERT_EQ(0U, probe_hash(&hash_table, k));
    int32_t depth = 25;
    int32_t score = 100;
    uint32_t age = 3;
    move_t m = to_move(PAWN, E2, E3);
    for (int i=0; i<NUM_HASH_SLOTS_PER_BUCKET; i++)
    {
        uint64_t v = build_hash_val(UPPER_BOUND, depth-i, score, m, age);
        store_hash_entry(&hash_table, k + (i*10), v);
    }

    // verify the values of the last entry 
    uint64_t last_key = k + (NUM_HASH_SLOTS_PER_BUCKET-1) * 10;
    uint64_t found = probe_hash(&hash_table, last_key);
    ASSERT_EQ(UPPER_BOUND, get_hash_entry_type(found));
    ASSERT_EQ(depth-NUM_HASH_SLOTS_PER_BUCKET+1, get_hash_entry_depth(found));
    ASSERT_EQ(score, get_hash_entry_score(found));
    ASSERT_EQ(m, get_hash_entry_move(found));
    ASSERT_EQ(age, get_hash_entry_age(found));

    // write one more with a smaller depth but same age.  it should displace the last entry.
    uint64_t v2 = build_hash_val(UPPER_BOUND, depth-NUM_HASH_SLOTS_PER_BUCKET, 101, NO_MOVE, age);
    store_hash_entry(&hash_table, k + NUM_HASH_SLOTS_PER_BUCKET * 10, v2);
    ASSERT_EQ(0U, probe_hash(&hash_table, last_key));

    // verify the initial entry
    uint64_t first_key = k;
    uint64_t found2 = probe_hash(&hash_table, first_key);
    ASSERT_EQ(UPPER_BOUND, get_hash_entry_type(found2));
    ASSERT_EQ(depth, get_hash_entry_depth(found2));
    ASSERT_EQ(score, get_hash_entry_score(found2));
    ASSERT_EQ(m, get_hash_entry_move(found2));
    ASSERT_EQ(age, get_hash_entry_age(found2));

    // write an entry with greater age.  it should displace the first entry.
    uint64_t v3 = build_hash_val(UPPER_BOUND, 1, 200, NO_MOVE, age+1);
    store_hash_entry(&hash_table, k + (NUM_HASH_SLOTS_PER_BUCKET+1)*10, v3);
    ASSERT_EQ(0U, probe_hash(&hash_table, first_key));
}
