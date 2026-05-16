#pragma once

char* prophet_strdup(const char* str);

#ifdef _WIN32

#include <stdio.h>
#include <stddef.h>

#ifndef ssize_t
typedef long long ssize_t;
#endif

ssize_t getline(char **lineptr, size_t *n, FILE *stream);
char* strtok_r(char* str, const char* delim, char** saveptr);

#endif
