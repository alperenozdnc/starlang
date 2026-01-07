#include <starlang/arena.h>

#include <assert.h>
#include <stdio.h>

void *arena_alloc(arena_t *arena, size_t size) {
    assert(arena != NULL);
    assert(size > 0);

    size_t aligned_size = ARENA_ALIGN_BLOCK(size);

    uintptr_t next = arena->cursor + aligned_size;
    assert(next <= arena->end);

    arena->cursor = next;

    return (void *)arena->cursor - aligned_size;
}
