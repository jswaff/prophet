#include <prophet/command.h>
#include <prophet/error_codes.h>
#include <prophet/parameters.h>
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
 * \param exit_status   Pointer to receive exit status.  A non-zero
 *                      status indicates the program should exit.
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int command_db(const char* input, int* exit_status)
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

    *exit_status = 0;

    return 0;
}
