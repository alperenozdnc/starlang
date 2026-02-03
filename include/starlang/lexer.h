#pragma once

#include <starlang/arena.h>
#include <starlang/transitions.h>

#include <stddef.h>

/*
 * the enum that lists all lexeme types.
 */
typedef enum {
#define X(name) name,
#include <starlang/lexemes.h>
    GET_LEXEMES()
#undef X
} lexeme_type_t;

static inline char *lexeme_to_str(lexeme_type_t t) {
    static char *table[] = {
#define X(name) #name,
#include <starlang/lexemes.h>
        GET_LEXEMES()
#undef X
    };

    return table[t];
}

/*
 * struct for holding information about a lexeme. this belongs to a region of
 * type `lexer_region_t`.
 */
typedef struct lexeme_t {
    lexeme_type_t type;

    char *view;
    size_t view_len;

    size_t line;
    size_t col;

    struct lexeme_t *next;
} lexeme_t;

/*
 * struct for holding information about a lexer region. a lexer region is a
 * range of lines that belong to a file with its corresponding lexemes.
 */
typedef struct lexer_region_t {
    size_t start;
    size_t end;

    lexeme_t *lexemes;

    char *filename;

    struct lexer_region_t *next;
} lexer_region_t;

/*
 * generates all regions for the source.
 */
lexer_region_t *lexer_generate_regions(arena_t *arena, src_t *source);

/*
 * appends a lexeme to the tail of a lexeme or creates a tail if none exists.
 */
void lexer_append_lexeme(arena_t *arena, lexer_region_t *region,
                         lexeme_t **tail);

/*
 * the lexer routine - this step in the pipeline converts all non-whitespace
 * tokens into 'lexemes'. you can think about lexemes as all tokens generalized
 * into the smallest categories possible without losing meaning.
 */
void lexer(arena_t *trans_arena, src_t *source);
