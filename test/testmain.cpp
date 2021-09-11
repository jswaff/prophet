#include <gtest/gtest.h>
#include <iostream>
#include <stdbool.h>

using namespace std;

extern "C" 
{
    extern bool logging_enabled;
    extern int init();
    extern int cleanup();
}

GTEST_API_ int main(int argc, char* argv[])
{
    cout << "Running main() from " << __FILE__ << endl;
    ::testing::InitGoogleTest(&argc, argv);

    logging_enabled = false;
    bool perft = false;

    for (int i=1; i<argc; i++) 
    {
        if (!strcmp("perft", argv[i])) 
        {
            perft = true;
        }
    }

    if (perft) 
    {
        ::testing::GTEST_FLAG(filter) = "perft_test*";
    } 
    else 
    {
        ::testing::GTEST_FLAG(filter) = "-perft_test*";

    }

    // initialize program data
    int retval = init();
    if (0 != retval)
    {
        goto cleanup_and_exit;
    }

    retval = RUN_ALL_TESTS();

cleanup_and_exit:

    cleanup();

    return retval;
}

