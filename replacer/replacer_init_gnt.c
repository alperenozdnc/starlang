#include <starlang/arena.h>
#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <string.h>

nmspc_link_t *replacer_init_gnt(arena_t *arena, const char *module) {
    nmspc_link_t *gnt_link = arena_alloc(arena, sizeof(*gnt_link));
    nmspc_node_t *gnt = arena_alloc(arena, sizeof(*gnt));
    memset(gnt, 0, sizeof(*gnt));

    gnt->namespace = util_build_str(arena, "(root)");
    gnt->path = util_build_str(arena, gnt->namespace, "/", module);
    gnt->module = (char *)module;
    gnt->parent = NULL;

    gnt_link->self = gnt;
    gnt_link->children = NULL;
    gnt_link->next = NULL;
    gnt_link->tail = NULL;

    return gnt_link;
}
