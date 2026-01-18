#include <starlang/replacer.h>

void replacer_append_child_to_link(nmspc_link_t *parent, nmspc_link_t *child) {
    if (parent->children) {
        parent->tail->next = child;
        parent->tail = child;

        return;
    }

    parent->children = child;
    parent->tail = child;

    return;
}
