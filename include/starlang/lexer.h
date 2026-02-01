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

/*
 * the lexer routine - this step in the pipeline converts all non-whitespace
 * tokens into 'lexemes'. you can think about lexemes as all tokens generalized
 * into the smallest categories possible without losing meaning.
 */
void lexer(arena_t *trans_arena, src_t *source);
