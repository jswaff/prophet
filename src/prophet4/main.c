#include <stdio.h>
#include <prophet/commandline.h>
#include <prophet/command.h>

extern int init();

/**
 * \brief Main entry point.
 *
 * \param argc          Number of arguments.
 * \param argv          List of arguments.
 *
 * \return 0 on successful execution, and non-zero on failure.
 */
int main(int argc, const char** argv)
{
    int retval = 0;

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    printf("Hello!  This is the Prophet4 Chess Engine.\n\n");

    retval = init();
    if (0 != retval)
    {
        goto done;
    }

    retval = process_commandline_options(argc, argv);
    if (0 != retval)
    {
        goto done;
    }

    retval = command_loop();

done:
    if (0 != retval)
    {
        printf("exiting with value: %d\n", retval);
    }

	return retval;
}
