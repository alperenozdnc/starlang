#pragma once

#include <starlang/arena.h>

#include <stddef.h>
#include <stdio.h>

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
 * the struct that holds all the information about a lexeme.
 */
typedef struct {
    lexeme_type_t kind;

    char *val_start;
    size_t val_size;

    size_t line;
    size_t col;
} lexeme_t;

/*
 * the struct that holds all the information about the lexer instance.
 */
typedef struct {
    char curr_char;
    FILE *curr_fptr;

    lexeme_t **lexemes;
    size_t lexeme_count;
} lexer_t;

/*
 * initalizes a lexer instance.
 */
lexer_t *lexer_init(arena_t *arena);
