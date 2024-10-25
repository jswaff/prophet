#include <prophet/hash.h>
#include "util/output.h"

bool cleanup_done = false;

/**
 * \brief Cleanup and exit
 *
 * \return 0 on successful execution, and non-zero on failure.
 */
int cleanup()
{
    if (!cleanup_done) {
        free_hash_tables();
        close_logfile();
        cleanup_done = true;
    }

    return 0;
}
