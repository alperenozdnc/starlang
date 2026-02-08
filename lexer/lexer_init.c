#include <starlang/lexer.h>
#include <starlang/transitions.h>
#include <starlang/utils.h>

#include <string.h>

lexer_t *lexer_init(src_t *source) {
    arena_t *a = arena_init(KiB(32));
    lexer_t *l = arena_alloc(a, sizeof(*l));
    memset(l, 0, sizeof(*l));

    l->arena = a;

    l->src = source->content;
    l->src_len = source->content_len;

    l->region = lexer_generate_regions(l->arena, source);
    l->base_region = l->region;

    return l;
}
