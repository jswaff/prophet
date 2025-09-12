#include "hash/hash_internal.h"

#include <gtest/gtest.h>

#include <stdlib.h>
#include <string.h>

TEST(hash_test, clear_table)
{
    // create a test hash table
    hash_table_t hash_table;
    hash_entry_t tbl[16];
    memset(tbl, 0, 16 * sizeof(hash_entry_t));
    hash_table.capacity = 16;
    hash_table.mask = 15;
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
    hash_table_t* hash_table = (hash_table_t*)malloc(sizeof(hash_table_t));
    hash_table->tbl = (hash_entry_t*)malloc(16 * sizeof(hash_entry_t));
    memset(hash_table->tbl, 0, 16 * sizeof(hash_entry_t));
    hash_table->capacity = 16;
    hash_table->mask = 15;

    // resize it large enough for 100 elements.  This will set the capacity to the largest power
    // of 2 possible to remain within the alloted size.
    uint32_t size_bytes = sizeof(hash_entry_t) * 100;
    resize_hash_table(hash_table, size_bytes);
    ASSERT_EQ(64U, hash_table->capacity);
    ASSERT_EQ(63U, hash_table->mask);

    // cleanup
    free(hash_table->tbl);
    free(hash_table);
}
