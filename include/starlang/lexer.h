#pragma once

#include <starlang/arena.h>
#include <starlang/transitions.h>
#include <starlang/utils.h>

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

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
    lexeme_t *tail_lexeme;

    char *filename;

    struct lexer_region_t *next;
} lexer_region_t;

typedef enum {
    LEX_MODE_NORMAL,
    LEX_MODE_NUMBER,
    LEX_MODE_STR,
    LEX_MODE_IDEN,
    LEX_MODE_CONSUME_LINE
} lexer_mode_t;

/*
 * struct for the holding information about the state of the lexer.
 */
typedef struct {
    char *src;
    size_t src_len;

    char *line_view;
    size_t line_len;

    size_t pos;
    size_t blacklist_idx;

    size_t offset;
    size_t line;
    size_t col;

    lexer_mode_t mode;
    lexer_region_t *region;
    arena_t *arena;
} lexer_t;

/*
 * checks and advances the region of the lexer if the current line index is
 * above the bounds of the current region.
 */
static inline void lexer_check_region(lexer_t *l) {
    if (l->region->next && ((l->line + l->offset) > l->region->end)) {
        l->region = l->region->next;
        l->offset = l->region->start;
        l->line = 0;
    }
}

/*
 * skips the current line.
 */
static inline bool lexer_skip_line(lexer_t *l, char c) {
    if (l->mode != LEX_MODE_CONSUME_LINE)
        return false;

    if (c == '\n' || c == EOF)
        l->mode = LEX_MODE_NORMAL;

    return true;
}

/*
 * gets the current line and allocates into arena.
 */
static inline char *lexer_get_line(lexer_t *l) {
    char *line_view = l->src + l->pos;

    size_t line_len = strcspn(line_view, "\n");
    char *line = arena_alloc(l->arena, line_len + 1);

    memcpy(line, line_view, line_len);

    line[line_len] = '\0';

    l->line_view = line;
    l->line_len = line_len;

    return line;
}

/*
 * peeks any character with a specific offset in the source content. returns
 * `'\0'` if bounds checking fails.
 */
static inline char lexer_peek(lexer_t *l, size_t offset) {
    return l->pos + offset < l->src_len ? l->src[l->pos + offset] : '\0';
}

/*
 * consumes any whitespace if lexer mode is normal.
 */
static inline bool lexer_consume_whitespace(lexer_t *l, char c) {
    return l->mode == LEX_MODE_NORMAL && isspace((unsigned char)c);
}

/*
 * consumes any comment if lexer mode is normal.
 */
static inline bool lexer_consume_comment(lexer_t *l) {
    if (l->mode != LEX_MODE_NORMAL)
        return false;

    if (l->line_view[0] == '#' ||
        util_does_str_end_with_suffix(l->line_view, "##")) {
        l->mode = LEX_MODE_CONSUME_LINE;

        return true;
    }

    return false;
}

#define LEXER_NO_MORE_IMPORTS ULONG_MAX

/*
 * consumes any import line.
 */
static inline bool lexer_consume_import(lexer_t *l, src_t *s) {
    if (l->blacklist_idx == LEXER_NO_MORE_IMPORTS)
        return false;

    size_t blacklisted = s->import_indices[l->blacklist_idx];

    if ((l->line + l->offset) == blacklisted) {
        bool in_bounds = l->blacklist_idx < s->import_indices_len;

        l->blacklist_idx =
            in_bounds ? l->blacklist_idx + 1 : LEXER_NO_MORE_IMPORTS;

        l->mode = LEX_MODE_CONSUME_LINE;

        return true;
    }

    return false;
}

/*
 * generates all regions for the source.
 */
lexer_region_t *lexer_generate_regions(arena_t *arena, src_t *source);

/*
 * appends a lexeme to the tail of a lexeme or creates a tail if none exists.
 */
void lexer_lexify_token(lexer_t *l, lexeme_type_t type, size_t len);

/*
 * initializes the lexer's state.
 */
lexer_t *lexer_init(src_t *source);

/*
 * advances lexer to the next character in source while keeping in mind the
 * line, column, position and region information.
 */
char lexer_continue(lexer_t *l);

/*
 * the lexer routine - this step in the pipeline converts all non-whitespace
 * tokens into 'lexemes'. you can think about lexemes as all tokens generalized
 * into the smallest categories possible without losing meaning.
 */
void lexer(arena_t *trans_arena, src_t *source);
