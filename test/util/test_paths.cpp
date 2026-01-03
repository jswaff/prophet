#include "util/test_paths.h"

#include <cstdlib>

std::string test_resource_path(const char* filename)
{
    const char* base_env = NULL;
#ifdef _WIN32
    char* env_buf = NULL;
    size_t env_len = 0;
    if (_dupenv_s(&env_buf, &env_len, "RESOURCE_DIR") == 0 && env_buf && env_len > 0) {
        base_env = env_buf;
    }
#else
    base_env = std::getenv("RESOURCE_DIR");
#endif
#ifdef RESOURCE_DIR
    const char* base_default = RESOURCE_DIR;
#else
    const char* base_default = "../test/resources";
#endif
    std::string base = base_env ? base_env : base_default;

    if (!base.empty()) {
        char last = base[base.size() - 1];
        if (last != '/' && last != '\\') {
            base += '/';
        }
    }

    std::string result = base + (filename ? filename : "");
#ifdef _WIN32
    free(env_buf);
#endif
    return result;
}
