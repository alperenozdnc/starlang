#include <starlang/arena.h>
#include <starlang/frontend.h>

#include <assert.h>
#include <stdlib.h>

void arena_resize(arena_t *arena, size_t size) {
    assert(arena != NULL);
    assert(size > 0);

    size_t new_size = ARENA_ALIGN_BLOCK(size);
    ssize_t diff_size = new_size - arena->capacity;

    arena = realloc(arena, sizeof(*arena) + size);

    if (!arena) {
        FRONTEND_PRINT(FRONTEND_ERR, FRONTEND_PRINT_PREFIX,
                       "failed to resize arena to %zu bytes.", size);

        return;
    }

    arena->capacity = new_size;
    arena->end += diff_size;
}
