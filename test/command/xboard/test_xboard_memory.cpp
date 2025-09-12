#include "command/xboard/xboard_internal.h"

#include "prophet/error_codes.h"

#include "hash/hash_internal.h"

#include <gtest/gtest.h>

#include <stdint.h>

extern hash_table_t htbl;
extern hash_table_t phtbl;

TEST(xboard_test, xboard_memory_incorrect_cmd)
{
    EXPECT_EQ(ERROR_CMD_INCORRECT_COMMAND, xboard_memory("bla"));
}

TEST(xboard_test, xboard_memory)
{
    uint32_t orig_capacity = htbl.capacity;

    ASSERT_EQ(0, xboard_memory("memory 64"));

    // the hash table capacity should have changed
    EXPECT_NE(0U, htbl.capacity);
    EXPECT_NE(orig_capacity, htbl.capacity);

    uint64_t expected_capacity = (uint64_t)(32 * 1024 * 1024) / sizeof(hash_entry_t);
    EXPECT_EQ(expected_capacity, htbl.capacity);
    EXPECT_EQ(expected_capacity, phtbl.capacity);
}
