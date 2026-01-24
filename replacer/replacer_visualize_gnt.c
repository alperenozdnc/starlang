#include <starlang/replacer.h>

#include <stdio.h>
#include <string.h>

void _replacer_visualize_gnt(nmspc_link_t *root, size_t depth) {
    if (root->self->parent) {
        printf("\n");
    }

    printf("%*s", (int)depth * 4, " ");

    printf("%s>%s",
           strlen(root->self->namespace) > 0 ? root->self->namespace : "(root)",
           root->self->module);

    if (root->children) {
        printf(" -> ");
    }

    nmspc_link_t *link = root->children;

    while (link) {
        _replacer_visualize_gnt(link, depth + 1);

        link = link->next;
    }

    if (!root->self->parent) {
        printf("\n");
    }

    if (!root->children && root->next) {
        printf(", ");
    }

    if (depth == 0) {
        printf("\n");
    }
}
