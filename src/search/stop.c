#include "search_internal.h"

#include "util/time.h"

#include <stdbool.h>

extern bool volatile stop_search;
extern bool volatile skip_time_checks;

void set_search_stop_flag(bool stop) {
    stop_search = stop;
}

void set_skip_time_checks_flag(bool skip) {
    skip_time_checks = skip;
}

/**
 * \brief Determine if the search should be stopped on time.
 *
 * \param opts          structure for tracking search options data
 * \param stats         structure for tracking search stats
 *
 * \return true if the search should be stopped, othwerwise false
 */
bool stop_search_on_time(search_options_t* opts, const stats_t* stats)
{
    /* if we don't have a stop time, nevermind! */
    if (!opts->stop_time) {
        return false;
    }

    /* avoid doing expensive time checks too often. */
    if (stats->nodes + stats->qnodes - opts->node_count_last_time_check < 
        opts->nodes_between_time_checks) 
    {
        return false;
    }

    /* ok, time check */
    opts->node_count_last_time_check = stats->nodes + stats->qnodes;
    return milli_timer() >= opts->stop_time;
}
