#include <starlang/arena.h>
#include <starlang/frontend.h>
#include <starlang/utils.h>

#include <stdio.h>
#include <string.h>

char *util_read_file_into_arena(arena_t *arena, FILE *f) {
    ssize_t file_size = util_get_file_size(f);

    char *buf = arena_alloc(arena, (size_t)file_size);

    ssize_t read_size = fread(buf, 1, (size_t)file_size, f);

    if (read_size != file_size) {
        perror("fread()");

        return NULL;
    }

    buf[file_size] = '\0';

    return buf;
}
