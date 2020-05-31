#include <prophet/error_codes.h>

#include <assert.h>
#include <pthread.h>

#include "xboard_internal.h"

/* externs */
extern bool stop_search;
extern pthread_t search_thread;
extern bool search_thread_running;
extern pthread_mutex_t search_lock;


/**
 * \brief Block on an active search thread.
 * 
 * Performs a join on any running search thread.  If the stop parameter
 * is true the search is forcefully stopped.
 *
 * \param stop          stop the search thread  
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int block_on_search_thread(bool stop)
{
    pthread_mutex_lock(&search_lock);
    if (search_thread_running)
    {
        if (stop) stop_search = true;
        int retval = pthread_join(search_thread, NULL);
        if (0 != retval)
        {
            return P4_ERROR_THREAD_JOIN_FAILURE;
        }
        search_thread_running = false;
    }
    pthread_mutex_unlock(&search_lock); 
    
    return 0;
}

