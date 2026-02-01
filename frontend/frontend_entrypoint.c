#include <starlang/arena.h>
#include <starlang/frontend.h>
#include <starlang/utils.h>

#include <starlang/lexer.h>
#include <starlang/replacer.h>

bool frontend_entrypoint(char *rel_main_module_path, char *parent_path,
                         char *filename) {
    FILE *file = fopen(rel_main_module_path, "r");
    size_t file_size = util_get_file_size(file);

    arena_t *trans_arena = arena_init(file_size);
    char *buf = util_read_file_into_arena(trans_arena, file);

    char *main_module_path = util_build_str(trans_arena, parent_path, filename);

    src_t *source = replacer(trans_arena, main_module_path, parent_path,
                             filename, buf, file_size - 1);

    lexer(trans_arena, source);

    arena_free(trans_arena);
    fclose(file);

    return true;
}
