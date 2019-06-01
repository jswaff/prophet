#ifndef _PRNG_H_
#define _PRNG_H_

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus


/**
 * \brief Generate a 32 bit random number.
 *
 * \return the 32 bit random number.
 */
uint32_t random32();


/**
 * \brief Generate a 64 bit random number.
 *
 * \return The 64 bit random number.
 */
uint64_t random64();


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _PRNG_H_ */
