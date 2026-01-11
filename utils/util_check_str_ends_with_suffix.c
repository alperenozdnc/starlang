#include <starlang/utils.h>

#include <stdbool.h>
#include <string.h>

bool util_does_str_end_with_suffix(const char *str, char *suffix) {
    char *result = strstr(str, suffix);

    return (result && result == str + strlen(str) - strlen(suffix));
}
