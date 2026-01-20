#include <starlang/replacer.h>
#include <stdio.h>

void replacer_print_flattened_gnt(nmspc_link_t **gnt_flat, size_t link_count) {
    printf("{ ");

    for (size_t i = 0; i < link_count; i++) {
        nmspc_node_t *node = gnt_flat[i]->self;

        printf("%s>%s", node->namespace, node->module);

        if (i + 1 < link_count) {
            printf(", ");
        }
    }

    printf(" }\n");
}
