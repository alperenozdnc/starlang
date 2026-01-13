#include <starlang/replacer.h>

#include <stdio.h>

void replacer_visualize_gnt(nmspc_link_t *root, size_t depth) {
    if (root->visualizer_visited)
        return;

    root->visualizer_visited = true;

    if (root->self->parent) {
        printf("\n");
    }

    printf("%*s", (int)depth * 4, " ");

    printf("%s/%s", root->self->namespace, root->self->module);

    if (root->children) {
        printf(" -> ");
    }

    nmspc_link_t *link = root->children;

    while (link) {
        replacer_visualize_gnt(link, depth + 1);

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
