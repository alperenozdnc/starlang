#include <starlang/utils.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

size_t util_get_file_size(FILE *f) {
    fseek(f, 0, SEEK_END);
    size_t file_size = ftell(f);
    rewind(f);

    return file_size;
}
