#include <starlang/replacer.h>

#include <stdio.h>

void replacer_visualize_gnt(nmspc_node_t *root, size_t depth) {
    if (root->parent) {
        printf("\n");
    }

    printf("%*s", (int)depth * 4, " ");
    fflush(stdout);

    printf("%s/%s", root->namespace, root->module);

    if (root->children[0]) {
        printf(" -> ");
    }

    size_t i = 0;

    while (root->children[i]) {
        replacer_visualize_gnt(root->children[i++], depth + 1);
    }

    if (!root->parent) {
        printf("\n");
    }

    if (!root->children[0]) {
        printf(", ");
    }
}
