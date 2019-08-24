#include <prophet/error_codes.h>
#include <prophet/position/position.h>
#include <prophet/util/string_utils.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern position gpos;

/**
 * \brief Execute the db (drawboard) command.
 *
 * Draw the board to stdout.
 *
 * \param input         the user entered input
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int command_db(const char* input)
{
    /* verify the command */
    if (0 != strcmp("db", input))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    char* buf = pos_to_str(&gpos);
    if (NULL == buf)
    {
        return P4_ERROR_CMD_DB_POS_TO_STR;
    }

    printf("%s\n", buf);

    free(buf);

    return 0;
}
