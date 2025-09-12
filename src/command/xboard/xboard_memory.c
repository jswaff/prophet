#include "xboard_internal.h"

#include "prophet/error_codes.h"

#include "hash/hash_internal.h"
#include "util/output.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern hash_table_t htbl;
extern hash_table_t phtbl;

/**
 * @brief Execute the xboard memory command 
 * 
 * @param input         the user entered input 
 *
 * @return 0 on successful execution, and non-zero on failure
 */
int xboard_memory(const char* input)
{
    /* verify the command */
    if (0 != strncmp("memory ", input, 7)) {
        return ERROR_CMD_INCORRECT_COMMAND;
    }

    /* is the command long enough to contain an argument? */
    if (strlen(input) < 8) {
        return ERROR_CMD_XBOARD_MEMORY_MISSING_SIZE;
    }

    /* attempt to read the SIZE parameter */
    uint32_t size_mb;
    if (1 != sscanf(input + 7,  "%u", &size_mb)) {
        return ERROR_CMD_XBOARD_MEMORY_MISSING_SIZE;
    }

    plog("# memory: %u mb; per table: %u mb\n", size_mb, size_mb/2);


    /* resize the hash tables */
    uint64_t size_bytes_per_table = (uint64_t)size_mb/2 * 1024 * 1024;
    int retval = resize_hash_table(&htbl, size_bytes_per_table);
    if (0 != retval) {
        return retval;
    }
    retval = resize_hash_table(&phtbl, size_bytes_per_table);

    return retval;
}
