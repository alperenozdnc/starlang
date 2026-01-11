#include <starlang/arena.h>

#include <assert.h>
#include <stdlib.h>

arena_t *arena_init(size_t capacity) {
    assert(capacity > 0);

    size_t aligned_capacity = ARENA_ALIGN_BLOCK(capacity);
    arena_t *arena = malloc(sizeof(*arena) + aligned_capacity);

    if (!arena)
        return NULL;

    arena->capacity = aligned_capacity;
    arena->start = (uintptr_t)arena + sizeof(*arena);
    arena->cursor = arena->start;
    arena->end = arena->start + aligned_capacity;
    arena->next = NULL;

    return arena;
}
