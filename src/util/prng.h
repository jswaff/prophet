#pragma once

#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


/**
 * \brief Generate a 32 bit random number.
 *
 * \return the 32 bit random number
 */
uint32_t random32();


/**
 * \brief Generate a 64 bit random number.
 *
 * \return the 64 bit random number
 */
uint64_t random64();


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  
