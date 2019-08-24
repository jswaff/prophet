#include <prophet/error_codes.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command_internal.h"

#define MAX_INPUT_SZ 256

/**
 * \brief Execute the command loop.
 *
 * Repeatedly reads, parses, and executes user commands until a command sets
 * the exit status flag.
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int command_loop()
{
    int retval = 0;

    char* input = (char*)malloc(MAX_INPUT_SZ);
    if (NULL == input)
    {
        return P4_ERROR_CMD_INPUT_BUFFER;
    }

    do
    {
        fgets(input, MAX_INPUT_SZ, stdin);

        /* remove trailing newline */
        if ((strlen(input) > 0) && (input[strlen(input) - 1] == '\n'))
        {
            input[strlen(input) - 1] = '\0';
        }

        retval = parse_and_execute(input);

    } while (retval == 0);


    free(input);

    return retval;
}
