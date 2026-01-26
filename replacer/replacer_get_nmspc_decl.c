#include <starlang/arena.h>
#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <stdlib.h>
#include <string.h>

nmspc_decl_t **replacer_get_nmspc_decl(arena_t *arena, char *parent_path) {
    char *full_path = util_build_str(arena, parent_path, ".starnmspc");

    FILE *decl_fptr = fopen(full_path, "r"); // todo: stop hardcoding

    if (!decl_fptr) {
        perror("fopen()");

        exit(EXIT_FAILURE);
    }

    size_t total_decl_line_size = util_count_lines(decl_fptr);
    char *decl_content = util_read_file_into_arena(arena, decl_fptr);

    nmspc_decl_t **declarations =
        arena_alloc(arena, sizeof(nmspc_decl_t *) * (total_decl_line_size + 1));

    size_t decl_idx = 0;

    size_t cursor = 0;
    size_t lines_size = 0;
    size_t line_len = 0;
    char *line = NULL;

    while ((line = util_read_line(arena, decl_content, strlen(decl_content),
                                  &line_len, &lines_size, &cursor)) != NULL) {
        bool reading_name = true;
        size_t max_ident_size = line_len + 1;

        char name[max_ident_size];
        char path[max_ident_size];
        memset(name, 0, max_ident_size);
        memset(path, 0, max_ident_size);

        size_t j = 0;
        size_t k = 0;

        for (size_t i = 0; i < line_len; i++) {
            char c = line[i];

            if (reading_name && c == ':') {
                reading_name = false;
                name[j] = '\0';

                continue;
            }

            if (reading_name) {
                name[j++] = c;

                continue;
            }

            path[k++] = c;
        }

        path[k] = '\0';

        nmspc_decl_t *decl = arena_alloc(arena, sizeof(*decl));

        char *alloc_namespace = util_build_str(arena, name);
        char *alloc_path = util_build_str(arena, path);

        decl->name = alloc_namespace;
        decl->path = alloc_path;

        declarations[decl_idx++] = decl;
    }

    declarations[decl_idx] = NULL;

    fclose(decl_fptr);

    return declarations;
}
