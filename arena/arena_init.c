#include <starlang/arena.h>

#include <assert.h>
#include <stdlib.h>

arena_t *arena_init(size_t capacity) {
    assert(capacity > 0);
    assert(capacity % ARENA_ALIGN_SIZE == 0); // not forced, just preference

    arena_t *arena = malloc(sizeof(*arena) + capacity);

    if (!arena)
        return NULL;

    arena->capacity = capacity;
    arena->start = (uintptr_t)arena + sizeof(*arena);
    arena->cursor = arena->start;
    arena->end = arena->start + capacity;

    return arena;
}
