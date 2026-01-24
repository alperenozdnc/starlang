#include <starlang/arena.h>
#include <starlang/frontend.h>
#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <stdio.h>

bool frontend_entrypoint(const char *rel_main_module_path,
                         const char *parent_path, const char *filename) {
    FILE *file = fopen(rel_main_module_path, "r");
    size_t file_size = util_get_file_size(file);

    arena_t *frontend_arena = arena_init(file_size);
    char *buf = util_read_file_into_arena(frontend_arena, file);

    char *main_module_path =
        util_build_str(frontend_arena, parent_path, filename);

    replacer(main_module_path, parent_path, filename, buf, file_size - 1);

    arena_free(frontend_arena);
    fclose(file);

    return true;
}
