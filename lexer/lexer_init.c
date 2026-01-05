#include <starlang/arena.h>
#include <starlang/lexer.h>

#include <string.h>

lexer_t *lexer_init(arena_t *arena) {
    lexer_t *lexer = arena_alloc(arena, sizeof(*lexer));
    memset(lexer, 0, sizeof(*lexer));

    return lexer;
}
