#include <starlang/arena.h>
#include <starlang/frontend.h>
#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <stdio.h>

bool frontend_entrypoint(const char *full_path, const char *path,
                         const char *filename) {
    FILE *file = fopen(full_path, "r");
    size_t file_size = util_get_file_size(file);

    arena_t *frontend_arena = arena_init(file_size);
    char *buf = util_read_file_into_arena(frontend_arena, file);

    replacer(path, filename, buf, file_size - 1);

    arena_free(frontend_arena);
    fclose(file);

    return true;
}
