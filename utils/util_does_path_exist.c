#include <starlang/utils.h>

#include <stdbool.h>
#include <unistd.h>

bool util_does_path_exist(char *pathname) {
    return access(pathname, F_OK) == 0; // 0 is the success code for access()
}
