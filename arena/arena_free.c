#include <starlang/arena.h>

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

void arena_free(arena_t *arena) {
    assert(arena != NULL);

    arena_t *node = arena;

    while (node != NULL) {
        arena_t *tmp = node;
        node = tmp->next;

        free(tmp);
    }
}
