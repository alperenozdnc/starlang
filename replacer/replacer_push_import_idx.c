#include <starlang/replacer.h>

#include <assert.h>
#include <stdlib.h>
#include <sys/param.h>

void replacer_push_import_idx(nmspc_node_t *node, size_t new_idx) {
    size_t **indices = &node->import_indices_heap;
    size_t *size = &node->import_indices_size;
    size_t len = node->import_indices_len;

    if (len + 1 <= *size)
        goto push_idx;

    *size = MAX(len + 1, *size * 2);
    *indices = realloc(*indices, sizeof(size_t) * *size);
    assert(*indices != NULL);

push_idx:
    node->import_indices_heap[len] = new_idx;
    node->import_indices_len++;
}
