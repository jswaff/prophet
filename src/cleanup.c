#include <prophet/hash.h>



/**
 * \brief Cleanup and exit
 *
 * \return 0 on successful execution, and non-zero on failure.
 */
int cleanup()
{
    free_hash_tables();

    return 0;
}
