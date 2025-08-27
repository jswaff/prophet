#include "prophet/nn.h"

#include "nn/nn_internal.h"

#include <gtest/gtest.h>

#include <stdbool.h>

extern neural_network_t neural_network;
extern bool use_neural_network;

TEST(nn_test, nn_load_network)
{
    ASSERT_EQ(0, load_network("../test/resources/nn.txt"));

    EXPECT_EQ(0, neural_network.W0[0]);
    EXPECT_EQ(1, neural_network.B1[0]);
    EXPECT_EQ(1, neural_network.B1[1]);
    EXPECT_TRUE(use_neural_network);

    // cleanup
    use_neural_network = false;
}
