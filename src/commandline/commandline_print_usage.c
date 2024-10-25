#include "commandline.h"

#include "../util/output.h"


/**
 * \brief Print command line options.
 */
void commandline_print_usage()
{
    out(stdout, "Usage: prophet4 [-r N] [-d N]\n");
    out(stdout, "\t-r N    run in random move selection mode for N moves.\n");
    out(stdout, "\t-d N    set maximum search depth N.\n");
}
