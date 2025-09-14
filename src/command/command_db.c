#include "command.h"

#include "prophet/error_codes.h"

#include "position/position.h"
#include "util/output.h"
#include "util/string_utils.h"

#include <stdlib.h>
#include <string.h>

extern position_t gpos;


int command_db(const char *input)
{
    /* verify the command */
    if (0 != strcmp("db", input)) {
        return ERROR_CMD_INCORRECT_COMMAND;
    }

    char* buf = pos_to_str(&gpos);
    if (NULL == buf) {
        return ERROR_CMD_DB_POS_TO_STR;
    }

    out(stdout, "%s\n", buf);

    free(buf);

    return 0;
}
