#include <prophet/hash.h>

#include <gtest/gtest.h>


TEST(hash_test, clear_table)
{
    // create a test hash table
    hash_table_t hash_table;
    hash_entry_t tbl[10];
    memset(tbl, 0, 10 * sizeof(hash_entry_t));
    hash_table.capacity = 10;
    hash_table.tbl = tbl;

    // store something in it
    uint64_t key = 5;
    uint64_t val = 0xFFU;
    ASSERT_EQ(0U, probe_hash(&hash_table, key));
    store_hash_entry(&hash_table, key, val);
    ASSERT_EQ(val, probe_hash(&hash_table, key));

    // clear the table and reprobe
    clear_hash_table(&hash_table);
    EXPECT_EQ(0U, hash_table.probes);
    EXPECT_EQ(0U, hash_table.hits);
    EXPECT_EQ(0U, hash_table.collisions);

    ASSERT_EQ(0U, probe_hash(&hash_table, key));
    EXPECT_EQ(1U, hash_table.probes);
    EXPECT_EQ(0U, hash_table.hits);
    EXPECT_EQ(0U, hash_table.collisions);
}


TEST(hash_test, resize_table)
{
    // create a test hash table.  Note the resize operation does a free, which assumes
    // dynamic memory allocation
    hash_table_t hash_table;
    hash_entry_t* tbl;
    tbl = (hash_entry_t*)malloc(10 * sizeof(hash_entry_t));
    memset(tbl, 0, 10 * sizeof(hash_entry_t));
    hash_table.capacity = 10;
    hash_table.tbl = tbl;

    // resize it large enough for 100 elements
    uint32_t size_bytes = sizeof(hash_entry_t) * 100;
    resize_hash_table(&hash_table, size_bytes);
    ASSERT_EQ(100U, hash_table.capacity);   

    // cleanup
    free(tbl);
}