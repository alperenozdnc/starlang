#pragma once

#include <starlang/arena.h>

#include <stddef.h>
#include <stdio.h>

/*
 * the enum that lists all lexeme types.
 */
typedef enum {
    LEX_EOF,
    LEX_SEMI,
    LEX_COMMENT,
    LEX_IMPORT,
    LEX_PERIOD,

    LEX_FOR,
    LEX_IF,
    LEX_RET,
    LEX_WHILE,
    LEX_WHEN,
    LEX_DECLARE,
    LEX_NOTHING,

    LEX_LPAREN,
    LEX_RPAREN,
    LEX_LBRACKET,
    LEX_RBRACKET,
    LEX_LSQUIRLY,
    LEX_RSQUIRLY,

    LEX_DQUOTE,
    LEX_QUOTE,

    LEX_EQ,
    LEX_PLUS,
    LEX_MINUS,
    LEX_SLASH,
    LEX_MULTIPLY,
    LEX_LESS_THAN,
    LEX_GREATER_THAN,

    LEX_IDENT,
    LEX_NUM,
    LEX_STRING,
    LEX_TRUE,
    LEX_FALSE
} lexeme_type_t;

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
