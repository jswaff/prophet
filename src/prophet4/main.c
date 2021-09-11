#include <prophet/command.h>
#include <prophet/commandline.h>
#include <prophet/util/output.h>

extern int init();
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
    if (0 != retval)
    {
        goto done;
    }

    retval = init();
    if (0 != retval)
    {
        goto done;
    }

    /* welcome message, indicates initializing was successful */
    plog("Hello!  This is the Prophet 4.0 Chess Engine.\n\n");

    retval = command_loop();

done:
    if (0 != retval)
    {
        plog("exiting with value: %d\n", retval);
    }

    cleanup();

    return retval;
}
