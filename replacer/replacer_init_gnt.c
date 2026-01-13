#include <starlang/arena.h>
#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <string.h>

nmspc_link_t *replacer_init_gnt(arena_t *arena, const char *module) {
    const char *namespace = util_build_str(arena, "(root)");
    const char *path = util_build_str(arena, namespace, "/", module);

    nmspc_link_t *gnt =
        replacer_init_nmspc_link(arena, NULL, path, namespace, module);

    return gnt;
}
