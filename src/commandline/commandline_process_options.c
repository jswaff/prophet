#include <prophet/commandline.h>
#include <prophet/error_codes.h>
#include <prophet/util/output.h>

#include <getopt.h>
#include <stdbool.h>

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
        {"random",      no_argument,    0, 'r'}
    };

    int long_index = 0;
    int opt = 0;
    while ((opt = getopt_long(argc, argv, "r",
        long_options, &long_index)) != -1)
    {
        switch (opt)
        {
            case 'r':
                out(stdout, "random mode enabled.\n");
                random_mode = true;
                break;
            default:
                commandline_print_usage();
                return P4_ERROR_INVALID_COMMAND_LINE_OPTION;
        }
    }

    return 0;
}
