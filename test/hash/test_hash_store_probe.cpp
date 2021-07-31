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
    EXPECT_EQ(0U, hash_table.collisions);


    // store using key < capacity
    uint64_t val = 0xFFU;
    ASSERT_EQ(0U, probe_hash(&hash_table, 5, 0));
    EXPECT_EQ(1U, hash_table.probes);
    EXPECT_EQ(0U, hash_table.hits);
    EXPECT_EQ(0U, hash_table.collisions);

    store_hash_entry(&hash_table, 5, val);
    ASSERT_EQ(val, probe_hash(&hash_table, 5, 0));
    EXPECT_EQ(2U, hash_table.probes);
    EXPECT_EQ(1U, hash_table.hits);
    EXPECT_EQ(0U, hash_table.collisions);


    // using key > capacity
    ASSERT_EQ(0U, probe_hash(&hash_table, 12, 0));
    EXPECT_EQ(3U, hash_table.probes);
    EXPECT_EQ(1U, hash_table.hits);
    EXPECT_EQ(0U, hash_table.collisions);

    store_hash_entry(&hash_table, 12, val);
    ASSERT_EQ(val, probe_hash(&hash_table, 12, 0));
    EXPECT_EQ(4U, hash_table.probes);
    EXPECT_EQ(2U, hash_table.hits);
    EXPECT_EQ(0U, hash_table.collisions);

    // key collision - this would pull from the same bucket, 
    // but the key doesn't match. 
    ASSERT_EQ(0U, probe_hash(&hash_table, 2, 0));
    EXPECT_EQ(5U, hash_table.probes);
    EXPECT_EQ(2U, hash_table.hits);
    EXPECT_EQ(1U, hash_table.collisions);

    // now store in the bucket we just probed
    store_hash_entry(&hash_table, 2, val);
    ASSERT_EQ(val, probe_hash(&hash_table, 2, 0));
    EXPECT_EQ(6U, hash_table.probes);
    EXPECT_EQ(3U, hash_table.hits);
    EXPECT_EQ(1U, hash_table.collisions);

    ASSERT_EQ(0U, probe_hash(&hash_table, 12, 0));
    EXPECT_EQ(7U, hash_table.probes);
    EXPECT_EQ(3U, hash_table.hits);
    EXPECT_EQ(2U, hash_table.collisions);
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
    ASSERT_EQ(0U, probe_hash(&hash_table, key, 0));
    ASSERT_EQ(0U, probe_hash(&hash_table, key, 1));

    // store a value.  It should go directly into slot 1.
    uint64_t val = build_hash_val(LOWER_BOUND, 7, 100, NO_MOVE, 1); 
    store_hash_entry(&hash_table, key, val);
    ASSERT_EQ(val, probe_hash(&hash_table, key, 0));
    ASSERT_EQ(0U, probe_hash(&hash_table, key, 1));

    // store another value using the same key, but a shallower depth.  It
    // should be written to the second slot
    uint64_t val2 = build_hash_val(LOWER_BOUND, 5, 99, NO_MOVE, 2);
    store_hash_entry(&hash_table, key, val2);
    ASSERT_EQ(val, probe_hash(&hash_table, key, 0));
    ASSERT_EQ(val2, probe_hash(&hash_table, key, 1));

    // store yet another value with the same key, but to a greater depth.  It
    // should overwrite entry 1.
    uint64_t val3 = build_hash_val(UPPER_BOUND, 8, 101, NO_MOVE, 3);
    store_hash_entry(&hash_table, key, val3);
    ASSERT_EQ(val3, probe_hash(&hash_table, key, 0));
    ASSERT_EQ(val2, probe_hash(&hash_table, key, 1));
}

