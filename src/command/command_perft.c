#include <string.h>
#include <stdio.h>

#include <prophet/command.h>
#include <prophet/error_codes.h>
#include <prophet/movegen.h>
#include <prophet/parameters.h>
#include <prophet/position/position.h>
#include <prophet/util/p4time.h>

extern position gpos;

/**
 * \brief Execute the perft command.
 * 
 * Command should be in the format "perft D", where D is depth.  The perft is
 * executed iteratively, starting with depth=1 and ending with depth=D.
 * The number of nodes and nodes-per-second are printed to stdin for each
 * iteration.
 *
 * \param input         The user entered input 
 * \param exit_status   Pointer to receive exit status.  A non-zero 
 *                      status indicates the program should exit.
 *
 * \returns 0 on successful execution, and non-zero on failure.
 */
int command_perft(const char* input, int* exit_status)
{
    *exit_status = 0;

    int depth;

    /* advance the base command pointer past the "perft " substring */
    if (1 != sscanf(input + 6, "%d", &depth))
    {
        return P4_ERROR_CMD_PERFT_DEPTH;
    }


    printf("running perft depth %d.\n", depth);
    for (int i=1; i <= depth; i++) {
        int32_t start_time = milli_timer();
        uint64_t nodes = perft(&gpos, i);
        int32_t elapsed = milli_timer() - start_time;
        printf("depth %d - nodes: %lu - rate: %lu kn/s\n", i, nodes,
            nodes / (elapsed+1)); // avoid div by 0 
    }

    return 0;
}
