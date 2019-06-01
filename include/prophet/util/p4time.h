#ifndef _P4TIME_H_
#define _P4TIME_H_

#include <stdint.h>

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus


/**
 * \brief Return the number of milliseconds since epoch.
 *
 * \return The number of milliseconds since epoch.
 */
int32_t milli_timer();

// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _P4TIME_H_ */
