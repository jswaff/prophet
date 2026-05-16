#include "posix_compat.h"

#include <stdlib.h>
#include <string.h>

char* prophet_strdup(const char* str)
{
    if (!str) {
        return NULL;
    }

    size_t len = strlen(str) + 1;
    char* copy = (char*)malloc(len);
    if (copy) {
        memcpy(copy, str, len);
    }
    return copy;
}

#ifdef _WIN32

ssize_t getline(char **lineptr, size_t *n, FILE *stream)
{
    if (!lineptr || !n || !stream) {
        return -1;
    }

    if (*lineptr == NULL || *n == 0) {
        *n = 128;
        *lineptr = (char*)malloc(*n);
        if (!*lineptr) {
            return -1;
        }
    }

    if (!fgets(*lineptr, (int)*n, stream)) {
        return -1;
    }

    size_t len = strlen(*lineptr);
    while (len > 0 && (*lineptr)[len - 1] != '\n' && !feof(stream)) {
        size_t new_size = *n * 2;
        char* new_buf = (char*)realloc(*lineptr, new_size);
        if (!new_buf) {
            return -1;
        }
        *lineptr = new_buf;
        *n = new_size;

        if (!fgets(*lineptr + len, (int)(*n - len), stream)) {
            break;
        }
        len = strlen(*lineptr);
    }

    return (ssize_t)len;
}

char* strtok_r(char* str, const char* delim, char** saveptr)
{
    return strtok_s(str, delim, saveptr);
}

#endif
