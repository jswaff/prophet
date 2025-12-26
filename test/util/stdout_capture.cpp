#include "util/stdout_capture.h"

#include <string.h>

int stdout_capture_begin(stdout_capture_t* cap, char* buffer, size_t size)
{
    if (!cap || !buffer || size == 0) {
        return -1;
    }

    memset(cap, 0, sizeof(*cap));
    cap->buffer = buffer;
    cap->size = size;

#ifdef _WIN32
    FILE* new_stdout = NULL;
    if (freopen_s(&new_stdout, "NUL", "a", stdout) != 0) {
        return -1;
    }
    cap->new_stdout = new_stdout;
#else
    if (freopen("/dev/null", "a", stdout) != stdout) {
        return -1;
    }
#endif

    if (setvbuf(stdout, buffer, _IOFBF, size) != 0) {
        return -1;
    }

    cap->active = true;
    return 0;
}

int stdout_capture_end(stdout_capture_t* cap)
{
    if (!cap || !cap->active) {
        return -1;
    }

#ifdef _WIN32
    if (freopen_s(&cap->new_stdout, "CON", "a", stdout) != 0) {
        return -1;
    }
#else
    if (freopen("/dev/tty", "a", stdout) != stdout) {
        return -1;
    }
#endif

    setvbuf(stdout, NULL, _IONBF, 0);
    cap->active = false;
    return 0;
}
