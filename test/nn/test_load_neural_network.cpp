#include <gtest/gtest.h>

#include "nn/nn_internal.h"

TEST(nn_test, load_neural_network)
{
    neural_network_t nn = {0};

    ASSERT_EQ(0, load_neural_network(&nn, "../test/resources/nn.txt"));

    EXPECT_EQ(-2, nn.W0[0]);
    EXPECT_EQ(2, nn.W0[NN_SIZE_L1]);
    EXPECT_EQ(120, nn.W1[NN_SIZE_L1 * 2 * NN_SIZE_L2 - 1]);
    EXPECT_EQ(2, nn.B1[0]);
}
