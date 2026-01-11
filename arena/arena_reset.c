#include <starlang/arena.h>

#include <assert.h>

void arena_reset(arena_t *arena) {
    assert(arena != NULL);

    arena_t *node = arena;

    while (node != NULL) {
        node->cursor = node->start;
        node = node->next;
    }
}
