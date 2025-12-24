#include "getopt_compat.h"

#ifdef _WIN32

#include <string.h>

char* optarg = NULL;
int optind = 1;
int opterr = 1;
int optopt = 0;

static int has_option_arg(const char* optstring, int opt)
{
    const char* p = optstring;
    while (*p) {
        if (*p == opt) {
            return p[1] == ':';
        }
        p++;
        if (*p == ':') {
            p++;
        }
    }
    return 0;
}

static int find_long_option(const struct option* longopts, const char* name, int* index_out)
{
    for (int i = 0; longopts[i].name; i++) {
        if (strcmp(longopts[i].name, name) == 0) {
            if (index_out) {
                *index_out = i;
            }
            return i;
        }
    }
    return -1;
}

int getopt_long(int argc, char* const argv[], const char* optstring,
    const struct option* longopts, int* longindex)
{
    if (optind >= argc) {
        return -1;
    }

    const char* arg = argv[optind];
    if (arg[0] != '-' || arg[1] == '\0') {
        return -1;
    }

    if (strcmp(arg, "--") == 0) {
        optind++;
        return -1;
    }

    if (arg[0] == '-' && arg[1] == '-') {
        const char* name = arg + 2;
        const char* eq = strchr(name, '=');
        char name_buf[128];
        if (eq) {
            size_t len = (size_t)(eq - name);
            if (len >= sizeof(name_buf)) {
                return '?';
            }
            memcpy(name_buf, name, len);
            name_buf[len] = '\0';
            name = name_buf;
        }

        int li = 0;
        int idx = find_long_option(longopts, name, &li);
        if (idx < 0) {
            optind++;
            return '?';
        }

        if (longindex) {
            *longindex = li;
        }

        optarg = NULL;
        if (longopts[idx].has_arg == required_argument) {
            if (eq) {
                optarg = (char*)(eq + 1);
            } else if (optind + 1 < argc) {
                optarg = argv[++optind];
            } else {
                optind++;
                return '?';
            }
        }

        optind++;
        if (longopts[idx].flag) {
            *longopts[idx].flag = longopts[idx].val;
            return 0;
        }
        return longopts[idx].val;
    }

    int opt = (unsigned char)arg[1];
    optopt = opt;
    optarg = NULL;

    if (has_option_arg(optstring, opt)) {
        if (arg[2] != '\0') {
            optarg = (char*)(arg + 2);
        } else if (optind + 1 < argc) {
            optarg = argv[++optind];
        } else {
            optind++;
            return '?';
        }
    }

    optind++;
    return opt;
}

#endif
