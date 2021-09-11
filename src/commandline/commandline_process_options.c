#include <prophet/commandline.h>
#include <prophet/error_codes.h>
#include <prophet/util/output.h>

#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

extern volatile uint32_t max_depth;
extern uint32_t hash_size;
extern bool random_mode;


/**
 * \brief Parse command line options.
 * 
 * \param argc          number of arguments
 * \param argv          list of arguments
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int commandline_process_options(int argc, char* argv[])
{

    /* specify options */
    static struct option long_options[] = {
        {"random",      no_argument,       0, 'r'},
        {"set depth",   required_argument, 0, 'd'},
        {"set hash size in mb", required_argument, 0, 'h'}
    };

    int long_index = 0;
    int opt = 0;
    while ((opt = getopt_long(argc, argv, "rd:h:",
        long_options, &long_index)) != -1)
    {
        switch (opt)
        {
            case 'r':
                plog("random mode enabled.\n");
                random_mode = true;
                break;
            case 'd':
                plog("setting max depth to %s\n", optarg);
                max_depth = atoi(optarg);
                break;
            case 'h':
                plog("setting hash table size to %s mb\n", optarg);
                hash_size = atoi(optarg) * 1024 * 1024;
                break;
            default:
                commandline_print_usage();
                return P4_ERROR_INVALID_COMMAND_LINE_OPTION;
        }
    }

    return 0;
}
