#include "commandline.h"

#include "util/output.h"


void commandline_print_usage()
{
    out(stdout, "Usage: prophet [l] [-r N] [-d N] [-h N] [-p N] [-e FILE] [-n FILE]\n");
    out(stdout, "\t-l      enable logging.\n");
    out(stdout, "\t-r N    run in random move selection mode for N moves.\n");
    out(stdout, "\t-d N    set maximum search depth N.\n");
    out(stdout, "\t-h MB   set hash table size to N mb.\n");
    out(stdout, "\t-p MB   set pawn hash table size to N mb.\n");
    out(stdout, "\t-e FILE load eval weights from file.\n");
    out(stdout, "\t-n FILE load neural network weights from file.\n");
}
