#include "prophet/init.h"

#include "command/command.h"
#include "commandline/commandline.h"
#include "util/output.h"

#include <stdio.h>

extern int cleanup();

/**
 * \brief Main entry point.
 *
 * \param argc          number of arguments
 * \param argv          list of arguments
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int main(int argc, char* argv[])
{
    int retval = 0;

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    retval = commandline_process_options(argc, argv);
    if (0 != retval) {
        goto done;
    }

    retval = init();
    if (0 != retval) {
        goto done;
    }

    /* welcome message, indicates initialization was successful */
    plog("Hello!  This is the Prophet Chess Engine, version 5.1-dev.\n\n");

    retval = command_loop();

done:
    if (0 != retval) {
        plog("exiting with value: %d\n", retval);
    }

    cleanup();

    return retval;
}
