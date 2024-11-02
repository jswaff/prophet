#include "prophet/hash.h"

#include "util/prng.h"


/**
 * \brief Initialize the zobrist keys
 *
 */
void init_zkeys() {

    for (int i=0;i<2;i++) { 
        /* color */
        for (int j=0;j<64;j++) { 
            /* squares */
            for (int k=1;k<=6;k++) {
                zkeys.pieces[k][i][j] = random64();
            }
        }

        zkeys.ptm[i] = random64();
    }

    for (int i=0;i<16;i++) { 
        /* castling rights */
        zkeys.casting_rights[i] = random64();
    }

    for (int i=0;i<65;i++) { 
        /* ep square */
        zkeys.ep[i] = random64();
    }

}
