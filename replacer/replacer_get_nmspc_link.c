#include <starlang/arena.h>
#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

nmspc_link_t *replacer_get_nmspc_link(arena_t *arena, nmspc_node_t *parent_node,
                                      nmspc_decl_t **declarations,
                                      const char *namespace,
                                      const char *module) {
    char *path = NULL;

    size_t i = 0;

    while (declarations[i++]) {
        nmspc_decl_t *decl = declarations[i - 1];

        if (strcmp(decl->name, namespace) != 0)
            continue;

        // 'spec/' obv hardcoded for now
        char *namespace_path_fs = util_build_str(arena, "spec/", decl->name);
        char *module_path_fs =
            util_build_str(arena, namespace_path_fs, "/", module);

        if (access(namespace_path_fs, F_OK) != 0) {
            fprintf(stderr, "can't find that namespace in disk.");

            exit(EXIT_FAILURE);
        }

        if (access(module_path_fs, F_OK) != 0) {
            fprintf(stderr, "can't find that module in disk.");

            exit(EXIT_FAILURE);
        }

        path = module_path_fs;

        break;
    }

    if (!path)
        return NULL;

    nmspc_link_t *link = arena_alloc(arena, sizeof(*link));
    nmspc_node_t *node = arena_alloc(arena, sizeof(*node));

    memset(link, 0, sizeof(*link));
    memset(node, 0, sizeof(*node));

    node->parent = parent_node;
    node->namespace = namespace;
    node->module = module;
    node->path = path;

    link->self = node;
    link->tail = NULL;
    link->children = NULL;
    link->next = NULL;
    link->visualizer_visited = false;

    return link;
}
