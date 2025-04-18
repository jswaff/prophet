#include <gtest/gtest.h>

#include <iostream>

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

    // initialize program data
    int retval = init();
    if (0 != retval) {
        goto cleanup_and_exit;
    }

    retval = RUN_ALL_TESTS();

cleanup_and_exit:

    cleanup();

    return retval;
}

