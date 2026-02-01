#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <stdlib.h>
#include <string.h>

void replacer_compile_src(src_t *src, arena_t *intermediate_arena,
                          nmspc_link_t **gnt_flat, size_t link_count) {
    size_t content_size = 1; // +1 for \0 at end
    size_t import_indices_len = 0;

    for (size_t i = 0; i < link_count; i++) {
        nmspc_node_t *node = gnt_flat[i]->self;

        content_size +=
            node->content_len + 1; // +1 for \n seperating every file

        import_indices_len += node->import_indices_len;
    }

    char *content = arena_alloc(intermediate_arena, content_size);
    size_t content_write_cursor = 0;

    size_t *import_indices =
        arena_alloc(intermediate_arena, sizeof(size_t) * import_indices_len);
    size_t import_indices_idx = 0;
    size_t line_offset = 0;

    file_range_t **file_ranges =
        arena_alloc(intermediate_arena, sizeof(file_range_t *) * link_count);

    for (size_t i = 0; i < link_count; i++) {
        nmspc_node_t *node = gnt_flat[i]->self;

        file_range_t *range = arena_alloc(intermediate_arena, sizeof(*range));
        file_ranges[i] = range;

        range->start = line_offset;

        memcpy(content + content_write_cursor, node->content,
               node->content_len);

        content_write_cursor += node->content_len;
        content[content_write_cursor++] = '\n';

        for (size_t j = 0; j < node->import_indices_len; j++) {
            import_indices[import_indices_idx++] =
                node->import_indices_heap[j] + line_offset;
        }

        line_offset += node->content_line_count;

        range->end =
            line_offset - 1; // this balances the fact that we
                             // seperate files with extra newlines, which
                             // mustn't be included in semantic line accounting

        range->file_path = util_build_str(intermediate_arena, node->path);

        free(node->import_indices_heap);
    }

    content[content_write_cursor] = '\0';

    src->content = content;
    src->content_len = content_size - 1; // discluding \0 in len calculation

    src->import_indices = import_indices;
    src->import_indices_len = import_indices_len;

    src->file_ranges = file_ranges;
    src->file_ranges_len = link_count;
}
