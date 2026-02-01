#pragma once

#include <starlang/arena.h>
#include <starlang/transitions.h>

#include <stddef.h>

/*
 * the enum that lists all lexeme types.
 */
typedef enum {
#define X(name) name,
#include <starlang/lexeme.h>
    GET_LEXEMES()
#undef X
} lexeme_type_t;

static inline char *lexeme_to_str(lexeme_type_t t) {
    static char *table[] = {
#define X(name) #name,
#include <starlang/lexeme.h>
        GET_LEXEMES()
#undef X
    };

    return table[t];
}

typedef struct lexeme_t {
    lexeme_type_t type;

    char *view;
    size_t view_len;

    size_t line;
    size_t col;

    struct lexeme_t *next;
} lexeme_t;

typedef struct lexer_region_t {
    size_t start;
    size_t end;

    lexeme_t *lexemes;

    char *filename;

    struct lexer_region_t *next;
} lexer_region_t;

/*
 * the lexer routine - this step in the pipeline converts all non-whitespace
 * tokens into 'lexemes'. you can think about lexemes as all tokens generalized
 * into the smallest categories possible without losing meaning.
 */
void lexer(arena_t *trans_arena, src_t *source);
