#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <prophet/command.h>
#include <prophet/error_codes.h>
#include <prophet/parameters.h>
#include <prophet/position/position.h>
#include <prophet/util/string_utils.h>

/**
 * \brief Execute the db (drawboard) command.
 *
 * Draw the board to stdin.
 *
 * \param input         The user entered input
 * \param exit_status   Pointer to receive exit status.  A non-zero
 *                      status indicates the program should exit.
 *
 * \returns 0 on successful execution, and non-zero on failure.
 */
int command_db(const char* UNUSED(input), int* exit_status)
{
    *exit_status = 0;

    // TODO: other positions
    position pos;
    reset_pos(&pos);

    char* buf = pos_to_str(&pos);
    if (NULL == buf)
    {
        return P4_ERROR_CMD_DB_POS_TO_STR;
    }

    printf("%s\n", buf);

    free(buf);

    return 0;
}
