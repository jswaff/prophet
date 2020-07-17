#include <prophet/error_codes.h>
#include <prophet/hash.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern hash_table_t htbl;

/**
 * \brief Execute the xboard memory command 
 * 
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_memory(const char* input)
{
    /* verify the command */
    if (0 != strncmp("memory ", input, 7))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    /* is the command long enough to contain an argument? */
    if (strlen(input) < 8)
    {
        return P4_ERROR_CMD_XBOARD_MEMORY_MISSING_SIZE;
    }

    /* attempt to read the SIZE parameter */
    uint32_t size_mb;
    if (1 != sscanf(input + 7, "%d", &size_mb))
    {
        return P4_ERROR_CMD_XBOARD_MEMORY_MISSING_SIZE;
    }


    /* resize the hash table */
    int retval = resize_hash_table(&htbl, size_mb * 1024 * 1024);


    return retval;
}
