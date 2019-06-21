#include <prophet/position/position.h>
#include <prophet/movegen.h>
#include <prophet/util/p4time.h>
#include <prophet/parameters.h>

#include <stdio.h>


void process_commandline_options(int UNUSED(argc), const char** UNUSED(argv))
{
    // just some temp code
    /*position pos;
    reset_pos(&pos);

    printf("running perft.\n");
    for (int i=1; i<7; i++) {
        int32_t start_time = milli_timer();
        uint64_t nodes = perft(&pos, i);
        int32_t elapsed = milli_timer() - start_time;
        printf("depth %d - nodes: %ld - rate: %ld n/s\n", i, nodes, 
            nodes * 1000 / (elapsed+1)); // avoid div by 0 
    }*/

}
