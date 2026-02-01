#include <starlang/arena.h>
#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <string.h>

nmspc_link_t *replacer_init_gnt(arena_t *arena, char *content,
                                size_t content_len, char *path, char *module) {
    nmspc_link_t *gnt = replacer_init_nmspc_link(arena, NULL, path, "", module);

    gnt->self->content = content;
    gnt->self->content_len = content_len;

    return gnt;
}
