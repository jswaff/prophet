#include <prophet/error_codes.h>
#include <prophet/eval.h>
#include <prophet/position/position.h>
#include <prophet/util/output.h>

#include <inttypes.h>
#include <string.h>

extern position_t gpos;

/**
 * \brief Execute the eval command.
 * 
 * Evaluate the current position and print the score.
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int command_eval(const char* input)
{
    /* verify the command */
    if (0 != strncmp("eval", input, 4)) {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    int32_t score = eval(&gpos, false);

    out(stdout, "eval score: %d\n", score);

    return 0;
}
