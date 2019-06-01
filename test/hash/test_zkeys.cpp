#include <gtest/gtest.h>

#include <prophet/hash.h>

TEST(hash_test, zkeys_initialized)
{
    for (int i=0;i<2;i++) { // color
        for (int j=0;j<64;j++) { // squares
            for (int k=1;k<=6;k++) {
                EXPECT_NE(zkeys.pieces[k][i][j], (uint64_t)0);
            }
        }

        EXPECT_NE(zkeys.ptm[i], (uint64_t)0);
    }

    for (int i=0;i<16;i++) { // castling rights
        EXPECT_NE(zkeys.casting_rights[i], (uint64_t)0);
    }

    for (int i=0;i<65;i++) { // ep square
        EXPECT_NE(zkeys.ep[i], (uint64_t)0);
    }
}
