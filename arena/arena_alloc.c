#include <starlang/arena.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/param.h>

void *arena_alloc(arena_t *arena, size_t size) {
    assert(arena != NULL);
    assert(size > 0);

    size_t aligned_size = ARENA_ALIGN_BLOCK(size);

    arena_t *prev_node = NULL;
    arena_t *node = arena;

    while (node != NULL) {
        uintptr_t next = node->cursor + aligned_size;

        if (next > node->end) {
            prev_node = node;
            node = node->next;

            continue;
        }

        node->cursor = next;

        return (void *)(node->cursor - aligned_size);
    }

    size_t node_size = MAX(prev_node->capacity * 2, aligned_size);
    arena_t *next = arena_init(node_size);
    prev_node->next = next;

    if (!next)
        return NULL;

    uintptr_t ptr = next->start;
    next->cursor = ptr + aligned_size;

    return (void *)ptr;
}
