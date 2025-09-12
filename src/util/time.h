#pragma once

#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


/**
 * @brief Return the number of milliseconds since epoch.
 *
 * @return the number of milliseconds since epoch
 */
uint64_t milli_timer();


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  
