#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct stdout_capture {
    char* buffer;
    size_t size;
#ifdef _WIN32
    FILE* new_stdout;
#endif
    bool active;
} stdout_capture_t;

int stdout_capture_begin(stdout_capture_t* cap, char* buffer, size_t size);
int stdout_capture_end(stdout_capture_t* cap);
