#include <stdio.h>
#include <prophet/commandline.h>
#include <prophet/command.h>

extern void init();

/**
 * \brief Main entry point.
 *
 * \param argc          Number of arguments.
 * \param argv          List of arguments.
 *
 * \returns 0 on successful execution, and non-zero on failure.
 */
int main(int argc, const char** argv)
{
    int retval = 0;

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    printf("Hello!  This is the Prophet4 Chess Engine.\n\n");

    // TODO: return val
    init();

    // TODO: return val
    process_commandline_options(argc, argv);

    retval = command_loop();

    printf("exiting with value: %d\n", retval);

	return retval;
}
