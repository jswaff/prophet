#include "prophet/nn.h"

#include "nn/nn_internal.h"

#include "test_globals.h"
#include "util/test_paths.h"

#include <gtest/gtest.h>

#include <stdbool.h>

TEST(nn_test, nn_load_network)
{
    std::string nn_path = test_resource_path("nn.txt");
    ASSERT_EQ(0, load_neural_network(nn_path.c_str()));

    EXPECT_EQ(0, neural_network.W0[0]);
    EXPECT_EQ(1, neural_network.B1[0]);
    EXPECT_EQ(1, neural_network.B1[1]);
    EXPECT_TRUE(use_neural_network);

    // cleanup
    use_neural_network = false;
}
