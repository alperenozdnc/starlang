#include <starlang/arena.h>
#include <starlang/utils.h>

#include <string.h>

char *util_dup_slice(arena_t *arena, char *view, size_t len) {
    char *slice = arena_alloc(arena, len + 1);

    memcpy(slice, view, len);
    slice[len] = '\0';

    return slice;
}
