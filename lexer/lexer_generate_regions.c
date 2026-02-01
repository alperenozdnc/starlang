#include <starlang/lexer.h>

#include <assert.h>

lexer_region_t *lexer_generate_regions(arena_t *arena, src_t *source) {
    assert(source->file_ranges_len > 0);

    lexer_region_t *base_region = arena_alloc(arena, sizeof(*base_region));
    lexer_region_t *tail = base_region;

    for (size_t i = 0; i < source->file_ranges_len; i++) {
        file_range_t *range = source->file_ranges[i];

        tail->start = range->start;
        tail->end = range->end;
        tail->filename = range->file_path;
        tail->lexemes = NULL;

        if (i + 1 < source->file_ranges_len) {
            tail->next = arena_alloc(arena, sizeof(*tail->next));
            tail = tail->next;
        } else {
            tail->next = NULL;
        }
    }

    return base_region;
}
