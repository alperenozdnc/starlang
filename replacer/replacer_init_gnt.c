#include <starlang/arena.h>
#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <stdlib.h>
#include <string.h>

nmspc_node_t *replacer_init_gnt(arena_t *arena, const char *module) {
    nmspc_node_t *gnt = arena_alloc(arena, sizeof(*gnt));
    memset(gnt, 0, sizeof(*gnt));

    gnt->namespace = util_build_str(arena, "(root)");
    gnt->path = util_build_str(arena, gnt->namespace, "/", module);
    gnt->module = (char *)module;

    gnt->parent = NULL;

    gnt->children =
        malloc(sizeof(nmspc_node_t *) * 1); // todo: stop using malloc here

    gnt->children[0] = NULL;

    return gnt;
}
