#include <starlang/arena.h>

#include <assert.h>

void arena_reset(arena_t *arena) {
    assert(arena != NULL);

    arena->cursor = arena->start;
}
