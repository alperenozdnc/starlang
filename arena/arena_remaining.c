#include <starlang/arena.h>

#include <assert.h>

size_t arena_remaining(arena_t *arena) {
    assert(arena != NULL);

    return (size_t)(arena->end - arena->cursor);
}
