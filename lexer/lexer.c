#include "starlang/frontend.h"
#include <starlang/arena.h>
#include <starlang/lexer.h>
#include <starlang/transitions.h>

void lexer(arena_t *trans_arena, src_t *source) {
    (void)trans_arena;

    lexer_t *l = lexer_init(source);

    char c = '\0';

    while ((c = lexer_continue(l)) != EOF) {
        if (l->mode == LEX_MODE_HIJACKED)
            continue;

        if (lexer_skip_line(l, c))
            continue;

        if (lexer_consume_import(l, source))
            continue;

        if (lexer_consume_comment(l))
            continue;

        if (lexer_lex_string(l, c))
            continue;

        if (lexer_lex_number(l, c))
            continue;

        if (lexer_lex_iden(l, c))
            continue;

        if (lexer_consume_whitespace(l, c))
            continue;

        if (lexer_lex_misc(l, c))
            continue;

        FRONTEND_THROW_TRACED_ERR_WITH_POS(
            l->region->filename, l->line_view, l->line + 1, l->col, 1,
            "unrecognized character '%c' at line %zu, col %zu", c, l->line + 1,
            l->col);
    }

    lexer_visualize(l);

    arena_free(l->arena);
}
