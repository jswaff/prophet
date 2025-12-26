#include "util/stdout_capture.h"

#include <string.h>

static const char* null_device_path()
{
#ifdef _WIN32
    return "NUL";
#else
    return "/dev/null";
#endif
}

static const char* tty_device_path()
{
#ifdef _WIN32
    return "CON";
#else
    return "/dev/tty";
#endif
}

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
    if (freopen_s(&new_stdout, null_device_path(), "a", stdout) != 0) {
        return -1;
    }
    cap->new_stdout = new_stdout;
#else
    if (freopen(null_device_path(), "a", stdout) != stdout) {
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
    if (freopen_s(&cap->new_stdout, tty_device_path(), "a", stdout) != 0) {
        return -1;
    }
#else
    if (freopen(tty_device_path(), "a", stdout) != stdout) {
        return -1;
    }
#endif

    setvbuf(stdout, NULL, _IONBF, 0);
    cap->active = false;
    return 0;
}
