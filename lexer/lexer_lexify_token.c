#include <starlang/arena.h>
#include <starlang/lexer.h>

#include <string.h>

static inline lexeme_t *alloc_lexeme_to_region(lexer_t *l) {
    if (l->region->lexemes == NULL) {
        l->region->lexemes = arena_alloc(l->arena, sizeof(*l->region->lexemes));

        l->region->tail_lexeme = l->region->lexemes;
    } else {
        l->region->tail_lexeme->next =
            arena_alloc(l->arena, sizeof(*l->region->lexemes));

        l->region->tail_lexeme = l->region->tail_lexeme->next;
    }

    memset(l->region->tail_lexeme, 0, sizeof(*l->region->tail_lexeme));

    return l->region->tail_lexeme;
}

lexeme_t *lexer_lexify_token(lexer_t *l, lexeme_type_t type, size_t len) {
    lexeme_t *lex = alloc_lexeme_to_region(l);

    lex->line = l->line;
    lex->col = l->col;

    lex->view = l->src + l->pos;
    lex->view_len = len;

    lex->type = type;
    lex->line_view = l->line_view;
    lex->line_view_len = l->line_len;
    lex->filename = l->region->filename;

    return lex;
}
