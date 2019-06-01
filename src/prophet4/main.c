#include <stdio.h>
#include <prophet/commandline.h>

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
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    printf("Hello!  This is the Prophet4 Chess Engine.\n\n");

    init();
    process_commandline_options(argc, argv);


	return 0;
}
