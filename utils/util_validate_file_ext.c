#include <starlang/utils.h>

#include <stdbool.h>
#include <string.h>

bool util_validate_file_ext(const char *pathname, const char *ext) {
    char *ext_str = strrchr(pathname, '.');

    if (!ext_str)
        return false;

    return strcmp(ext_str, ext) == 0;
}
