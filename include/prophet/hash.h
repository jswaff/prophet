#ifndef _HASH_H_
#define _HASH_H_

#include <stdint.h>

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus

typedef struct {
    uint64_t pieces[7][2][64];
    uint64_t ptm[2];
    uint64_t casting_rights[16];
    uint64_t ep[65]; // include NO_SQUARE
} zobrist_keys;

extern zobrist_keys zkeys;


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _HASH_H_ */

