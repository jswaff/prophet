#include "util/output.h"

bool cleanup_done = false;

extern void free_hash_tables();


int cleanup()
{
    if (!cleanup_done) {
        free_hash_tables();
        close_logfile();
        cleanup_done = true;
    }

    return 0;
}
