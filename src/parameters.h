#pragma once

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


#ifdef   __GNUC__
# define UNUSED(x) x __attribute__((__unused__))
#else
# define UNUSED(x) x
#endif 

/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif    
