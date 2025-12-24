#include "xboard_internal.h"

#include "prophet/error_codes.h"

#include <assert.h>
#include "util/thread_compat.h"

/* externs */
extern volatile bool stop_search;
extern prophet_thread_t search_thread;
extern bool search_thread_running;
extern prophet_mutex_t search_lock;
void ensure_xboard_threading_initialized();


int block_on_search_thread(bool stop)
{
    ensure_xboard_threading_initialized();
    prophet_mutex_lock(&search_lock);
    if (search_thread_running) {
        if (stop) stop_search = true;
        int retval = prophet_thread_join(search_thread);
        if (0 != retval) {
            return ERROR_THREAD_JOIN_FAILURE;
        }
        search_thread_running = false;
    }
    prophet_mutex_unlock(&search_lock);
    
    return 0;
}
