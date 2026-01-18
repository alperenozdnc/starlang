#include <starlang/replacer.h>

size_t _replacer_get_gnt_link_count(nmspc_link_t *parent, size_t link_count) {
    if (!parent)
        return link_count;

    size_t tmp = link_count;
    nmspc_link_t *cursor = parent;

    while (cursor) {
        tmp = _replacer_get_gnt_link_count(cursor->children, tmp + 1);

        cursor = cursor->next;
    }

    return tmp;
}
