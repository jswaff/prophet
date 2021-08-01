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
    uint64_t val = build_hash_val(LOWER_BOUND, 7, 100, NO_MOVE, 1); 
    store_hash_entry(&hash_table, key, val);
    ASSERT_EQ(val, probe_hash(&hash_table, key));

    // store another value using the same key, with the same age and
    // a greater depth.  Because it uses the same key, it should 
    // overwrite the previous entry.
    uint64_t key2 = key;
    uint64_t val2 = build_hash_val(LOWER_BOUND, 9, 99, NO_MOVE, 1);
    store_hash_entry(&hash_table, key2, val2);
    ASSERT_EQ(val2, probe_hash(&hash_table, key2));


    // store another value with a different key, and a greater depth
    // than the previous entry, and the same age. 
    uint64_t key3 = 3;
    uint64_t val3 = build_hash_val(UPPER_BOUND, 10, -76, NO_MOVE, 1);
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

