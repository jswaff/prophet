#include <prophet/commandline.h>
#include <prophet/error_codes.h>
#include <prophet/util/output.h>

#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

extern volatile uint32_t max_depth;
extern bool logging_enabled;
extern uint64_t hash_size;
extern uint64_t pawn_hash_size;
extern uint32_t num_random_starting_moves;


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
        {"enable logging", no_argument,       0, 'l' },
        {"random mode",    required_argument, 0, 'r'},
        {"set depth",      required_argument, 0, 'd'},
        {"set hash size in mb", required_argument, 0, 'h'},
        {"set pawn hash size in mb", required_argument, 0, 'p'},
        {"eval properties", required_argument, 0, 'e'},
    };

    int long_index = 0;
    int opt = 0;
    while ((opt = getopt_long(argc, argv, "lr:d:h:p:e:", long_options, &long_index)) != -1) {
        switch (opt) {
            case 'l':
                logging_enabled = true;
                out(stdout, "logging enabled\n");
                break;
            case 'r':
                out(stdout, "random mode enabled for %s moves\n", optarg);
                num_random_starting_moves = atoi(optarg);
                break;
            case 'd':
                out(stdout, "setting max depth to %s\n", optarg);
                max_depth = atoi(optarg);
                break;
            case 'h':
                out(stdout, "setting hash table size to %s mb\n", optarg);
                hash_size = atoll(optarg) * 1024 * 1024;
                break;
            case 'p':
                out(stdout, "setting pawn hash table size to %s mb\n", optarg);
                pawn_hash_size = atoll(optarg) * 1024 * 1024;
                break;
            case 'e':
                out(stdout, "loading eval properties from %s\n", optarg);
                return commandline_load_properties(optarg);
            default:
                commandline_print_usage();
                return P4_ERROR_INVALID_COMMAND_LINE_OPTION;
        }
    }

    return 0;
}
