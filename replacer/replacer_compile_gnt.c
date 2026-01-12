#include <starlang/arena.h>
#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void replacer_append_child_to_node(nmspc_node_t *parent, nmspc_node_t *child) {
    // todo: use a linked list instead of an array which is better at
    // fitting inside arena constraints

    parent->children_count++;

    parent->children =
        realloc(parent->children,
                sizeof(nmspc_node_t *) * (parent->children_count + 1));

    parent->children[parent->children_count - 1] = child;
    parent->children[parent->children_count] = NULL;
}

void replacer_compile_gnt(arena_t *arena, nmspc_decl_t **decl,
                          nmspc_node_t *parent_node, const char *content,
                          size_t content_len) {
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

        replacer_enforce_import_syntax(line, lines_size, line_len, rhs, rhs_len,
                                       lhs_len);

        nmspc_module_t *dep_info =
            replacer_build_nmspc_and_module(arena, rhs, rhs_len);

        replacer_enforce_import_grammar(line, lines_size, dep_info->namespace,
                                        dep_info->module, lhs_len);

        nmspc_node_t *node = replacer_get_nmspc_node(
            arena, parent_node, decl, dep_info->namespace, dep_info->module);

        replacer_append_child_to_node(parent_node, node);

        FILE *f = fopen(node->path, "r");
        assert(f != NULL);

        size_t file_size = util_get_file_size(f);
        char *file_content = util_read_file_into_arena(arena, f);

        replacer_compile_gnt(arena, decl, node, file_content, file_size);

        fclose(f);
    }
}
