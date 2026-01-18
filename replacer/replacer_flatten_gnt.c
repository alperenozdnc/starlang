#include <starlang/replacer.h>

#include <string.h>

nmspc_link_t **_replacer_flatten_gnt(arena_t *arena, nmspc_link_t **list,
                                     nmspc_link_t *parent, size_t *link_idx,
                                     size_t link_count) {
    nmspc_link_t *cursor = parent;

    while (cursor) {
        _replacer_flatten_gnt(arena, list, cursor->children, link_idx,
                              link_count);

        list[*link_idx] = cursor;
        (*link_idx)++;

        cursor = cursor->next;
    }

    return list;
}

nmspc_link_t **replacer_flatten_gnt(arena_t *arena, nmspc_link_t *gnt,
                                    size_t link_count) {
    size_t link_idx = 0;

    size_t list_size = link_count * sizeof(nmspc_link_t *);
    nmspc_link_t **gnt_flat = arena_alloc(arena, list_size);
    memset(gnt_flat, 0, list_size);

    _replacer_flatten_gnt(arena, gnt_flat, gnt, &link_idx, link_count);

    return gnt_flat;
}
