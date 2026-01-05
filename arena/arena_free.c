#include <starlang/arena.h>

#include <assert.h>
#include <stdlib.h>

void arena_free(arena_t *arena) {
    assert(arena != NULL);

    free(arena);
}
