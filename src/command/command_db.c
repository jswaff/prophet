#include "command.h"

#include "prophet/error_codes.h"

#include "position/position_internal.h"
#include "util/output.h"
#include "util/string_utils.h"

#include <stdlib.h>
#include <string.h>

extern position_t gpos;

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
    if (0 != strcmp("db", input)) {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    char* buf = pos_to_str(&gpos);
    if (NULL == buf) {
        return P4_ERROR_CMD_DB_POS_TO_STR;
    }

    out(stdout, "%s\n", buf);

    free(buf);

    return 0;
}
