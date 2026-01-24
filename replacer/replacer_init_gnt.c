#include <starlang/arena.h>
#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <string.h>

nmspc_link_t *replacer_init_gnt(arena_t *arena, const char *path,
                                const char *module) {
    return replacer_init_nmspc_link(arena, NULL, path, "", module);
}
