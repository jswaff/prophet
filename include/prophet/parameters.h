#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus

#ifdef   __GNUC__
# define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
# define UNUSED(X) UNUSED_ ## x
#endif //__GNUC__

// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _PARAMETERS_H_ */
