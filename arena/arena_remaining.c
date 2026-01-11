#include <starlang/arena.h>

#include <assert.h>

size_t arena_remaining(arena_t *arena) {
    assert(arena != NULL);

    size_t remaining = 0;
    arena_t *node = arena;

    while (node != NULL) {
        remaining += (size_t)(node->end - node->cursor);
        node = node->next;
    }

    return remaining;
}
