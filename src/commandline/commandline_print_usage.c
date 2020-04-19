#include <prophet/util/output.h>


/**
 * \brief Print command line options.
 */
void commandline_print_usage()
{
    out(stdout, "Usage: prophet4 [-r] [-d N]\n");
    out(stdout, "\t-r      run in random move selection mode.\n");
    out(stdout, "\t-d N    set maximum search depth N.\n");
}
