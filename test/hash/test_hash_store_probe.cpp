#include <prophet/hash.h>

#include <gtest/gtest.h>

TEST(hash_test, store_and_probe)
{
    hash_table_t hash_table;

    // define a small table
    hash_entry_t tbl[10];
    memset(tbl, 0, 10 * sizeof(hash_entry_t));
    hash_table.capacity = 10;
    hash_table.tbl = tbl;

    // store using key < capacity
    uint64_t val = 0xFFU;
    ASSERT_EQ(0U, probe_hash(&hash_table, 5));
    store_hash_entry(&hash_table, 5, val);
    ASSERT_EQ(val, probe_hash(&hash_table, 5));

    // using key > capacity
    ASSERT_EQ(0U, probe_hash(&hash_table, 12));
    store_hash_entry(&hash_table, 12, val);
    ASSERT_EQ(val, probe_hash(&hash_table, 12));

    // key collision - this would pull from the same bucket, 
    // but the key doesn't match. 
    ASSERT_EQ(0U, probe_hash(&hash_table, 2));

    // now store in the bucket we just probed
    store_hash_entry(&hash_table, 2, val);
    ASSERT_EQ(val, probe_hash(&hash_table, 2));
    ASSERT_EQ(0U, probe_hash(&hash_table, 12));
}

TEST(hash_test, always_replace_replacement_strategy)
{
    hash_table_t hash_table;

    // define a small table
    hash_entry_t tbl[10];
    memset(tbl, 0, 10 * sizeof(hash_entry_t));
    hash_table.capacity = 10;
    hash_table.tbl = tbl;

    // store a value
    uint64_t val = 0xFFU;
    ASSERT_EQ(0U, probe_hash(&hash_table, 5));
    store_hash_entry(&hash_table, 5, val);
    ASSERT_EQ(val, probe_hash(&hash_table, 5));

    // store another value using the same key
    uint64_t val2 = 0xEEU;
    store_hash_entry(&hash_table, 5, val2);
    ASSERT_EQ(val2, probe_hash(&hash_table, 5));
}

