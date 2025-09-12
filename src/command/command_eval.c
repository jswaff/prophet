#include "command.h"

#include "prophet/error_codes.h"

#include "eval/eval_internal.h"
#include "nn/nn_internal.h"
#include "position/position.h"
#include "util/output.h"

#include <stdint.h>
#include <string.h>

extern neural_network_t neural_network;
extern bool use_neural_network;
extern position_t gpos;

/**
 * @brief Execute the eval command.
 * 
 * Evaluate the current position and print the score.
 *
 * @param input         the user entered input 
 *
 * @return 0 on successful execution, and non-zero on failure
 */
int command_eval(const char* input)
{
    /* verify the command */
    if (0 != strncmp("eval", input, 4)) {
        return ERROR_CMD_INCORRECT_COMMAND;
    }

    int32_t score = eval(&gpos, false, false);

    out(stdout, "eval score: %d\n", score);

    if (use_neural_network) {
        int32_t nn_score = nn_eval(&gpos, &neural_network);
        out(stdout, "nn score: %d\n", nn_score);
    }


    return 0;
}
