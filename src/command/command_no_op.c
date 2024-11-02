#include "command.h"

#include "parameters.h"

/**
 * \brief Execute the no op command - does nothing!
 * 
 * \param input         the user entered input (unused)
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int command_no_op(const char* UNUSED(input))
{
    return 0;
}
