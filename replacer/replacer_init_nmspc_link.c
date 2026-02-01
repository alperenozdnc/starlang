#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <string.h>

nmspc_link_t *replacer_init_nmspc_link(arena_t *arena, nmspc_node_t *parent,
                                       char *path, char *namespace,
                                       char *module) {
    nmspc_link_t *link = arena_alloc(arena, sizeof(*link));
    nmspc_node_t *node = arena_alloc(arena, sizeof(*node));

    memset(link, 0, sizeof(*link));
    memset(node, 0, sizeof(*node));

    node->namespace = namespace;
    node->path = path;
    node->module = module;
    node->parent = parent;

    node->content = NULL;
    node->content_len = 0;
    node->content_line_count = 0;

    node->import_indices_heap = NULL;
    node->import_indices_size = 0;
    node->import_indices_len = 0;

    link->self = node;
    link->children = NULL;
    link->next = NULL;
    link->tail = NULL;

    return link;
}
