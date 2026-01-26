#include <starlang/utils.h>

#include <stdbool.h>
#include <sys/stat.h>

bool util_is_path_a_file(char *pathname) {
    struct stat s = {0};

    stat(pathname, &s);

    return S_ISREG(s.st_mode);
}
