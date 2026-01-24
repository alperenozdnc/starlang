#include <starlang/arena.h>
#include <starlang/frontend.h>
#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void replacer_compile_gnt(arena_t *arena, const char *parent_dir,
                          nmspc_decl_t **decl, nmspc_link_t *root,
                          nmspc_link_t *parent, const char *content,
                          size_t content_len, size_t *link_count) {
    size_t cursor_pos = 0;
    size_t lines_size = 0;
    size_t line_len = 0;
    char *line = NULL;

    // lhs is always '@import ' in all cases so this is constant
    size_t lhs_len = IMPORT_ACTION_LEN + 1; // +1 for space after '@import'

    while ((line = util_read_line(arena, content, content_len, &line_len,
                                  &lines_size, &cursor_pos)) != NULL) {

        if (!replacer_heuristic_is_action(line, line_len))
            continue;

        char *rhs = line + lhs_len;
        size_t rhs_len = line_len - lhs_len;

        replacer_enforce_import_syntax(parent->self->path, line, lines_size,
                                       line_len, rhs, rhs_len, lhs_len);

        nmspc_module_t *dep_info =
            replacer_build_nmspc_and_module(arena, rhs, rhs_len);

        replacer_enforce_import_grammar(parent->self->path, line, lines_size,
                                        dep_info->namespace, dep_info->module,
                                        lhs_len);

        replacer_enforce_no_self_import((char *)parent->self->path, line,
                                        lines_size, line_len, parent, dep_info);

        if (!replacer_is_dep_uniq(root, dep_info))
            continue;

        nmspc_link_t *link = replacer_get_nmspc_link(
            arena, parent_dir, parent->self, decl, lines_size, line_len, line,
            dep_info->namespace, dep_info->module);

        (*link_count)++;
        replacer_append_child_to_link(parent, link);

        FILE *f = fopen(link->self->path, "r");
        assert(f != NULL);

        size_t file_size = util_get_file_size(f);
        char *file_content = util_read_file_into_arena(arena, f);

        replacer_compile_gnt(arena, parent_dir, decl, root, link, file_content,
                             file_size, link_count);

        fclose(f);
    }
}
