#include <starlang/replacer.h>

#include <stdio.h>
#include <string.h>

void replacer_print_flattened_gnt(nmspc_link_t **gnt_flat, size_t link_count) {
    printf("the flattened GNT, showing all dependencies ordered in a way such "
           "that, going from left to \n"
           "right, no dependency doesn't have access to any subdependency it \n"
           "requires, keeping no more than a single entry per dependency:\n\n");

    printf("{ ");

    for (size_t i = 0; i < link_count; i++) {
        nmspc_node_t *node = gnt_flat[i]->self;

        if (node->parent)
            printf("%s>", node->namespace);

        printf("%s", node->module);

        if (i + 1 < link_count) {
            printf(", ");
        }
    }

    printf(" }\n");
}
