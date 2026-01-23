#include <starlang/arena.h>
#include <starlang/frontend.h>
#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

nmspc_link_t *replacer_get_nmspc_link(arena_t *arena, const char *parent_path,
                                      nmspc_node_t *parent_node,
                                      nmspc_decl_t **declarations,
                                      size_t line_idx, size_t line_len,
                                      const char *line, const char *namespace,
                                      const char *module) {
    char *link_path = NULL;
    size_t i = 0;

    while (declarations[i++]) {
        nmspc_decl_t *decl = declarations[i - 1];

        if (strcmp(decl->name, namespace) != 0)
            continue;

        char *namespace_path_fs =
            util_build_str(arena, parent_path, decl->name);
        char *module_path_fs =
            util_build_str(arena, namespace_path_fs, "/", module);

        // +1 for making column 1-initialized
        // +1 for skipping the space after '@import'
        // +1 for skipping the '>' in syntax namespace>module
        size_t col_start = IMPORT_ACTION_LEN + strlen(namespace) + 3;

        if (access(module_path_fs, F_OK) != 0) {
            FRONTEND_THROW_ERR_WITH_POS((char *)line, line_idx, col_start,
                                        line_len - col_start,
                                        "can't find module '%s' from namespace "
                                        "'%s' on disk imported from line %zu.",
                                        module, namespace, line_idx);
        }

        link_path = module_path_fs;

        break;
    }

    if (link_path == NULL) {
        // +1 for making column 1-initialized
        // +1 for skipping the space after '@import'
        size_t col_start = IMPORT_ACTION_LEN + 2;

        FRONTEND_THROW_ERR_WITH_POS(
            (char *)line, line_idx, col_start, strlen(namespace),
            "can't find namespace '%s' on disk imported from line %zu.",
            namespace, line_idx);
    }

    nmspc_link_t *link = replacer_init_nmspc_link(arena, parent_node, link_path,
                                                  namespace, module);

    return link;
}
