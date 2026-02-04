#include <starlang/arena.h>
#include <starlang/lexer.h>
#include <starlang/transitions.h>

void lexer(arena_t *trans_arena, src_t *source) {
    (void)trans_arena;

    lexer_t *l = lexer_init(source);

    return;
}
