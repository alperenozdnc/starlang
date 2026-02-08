#pragma once

#include <starlang/arena.h>
#include <starlang/frontend.h>
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
    char *line_view;
    size_t line_view_len;

    size_t line;
    size_t col;
    size_t pos;

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
    LEX_MODE_CONSUME_LINE,
    LEX_MODE_HIJACKED
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
    lexer_region_t *base_region;
    lexer_region_t *region;
    arena_t *arena;

    bool is_str_dbquot;
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
static inline char lexer_peek(lexer_t *l, int offset) {
    return (l->pos + offset) < l->src_len ? l->src[l->pos + offset] : '\0';
}

/*
 * consumes any whitespace if lexer mode is normal.
 */
static inline bool lexer_consume_whitespace(lexer_t *l, char c) {
    return l->mode == LEX_MODE_NORMAL && isspace((unsigned char)c);
}

/*
 * advances lexer to the next character in source while keeping in mind the
 * line, column, position and region information.
 */
char lexer_continue(lexer_t *l);

/*
 * consumes any comment if lexer mode is normal.
 */
static inline bool lexer_consume_comment(lexer_t *l) {
    if (l->mode != LEX_MODE_NORMAL)
        return false;

    bool singleline_comment = (l->line_view[0] == '#');
    bool multiline_comment =
        (singleline_comment && (l->line_len > 1 && l->line_view[1] == '#'));

    // multiline case has singleline as true too, no need for redundant check
    if (!singleline_comment)
        return false;

    if (!multiline_comment) {
        l->mode = LEX_MODE_CONSUME_LINE;

        return true;
    }

    size_t comment_len = 0; // includes the character we're on

    for (size_t i = l->pos; i < l->src_len; i++) {
        char c = l->src[i];
        char cn = (i < l->src_len - 1) ? l->src[i + 1] : EOF;

        if (cn == EOF)
            goto eof_err;

        comment_len++;

        if (c == '#' && c == cn)
            break;
    }

    l->mode = LEX_MODE_HIJACKED;

    size_t consumed_len = 0;
    while ((consumed_len++) <= comment_len)
        lexer_continue(l);

    l->mode = LEX_MODE_NORMAL;

    return true; // not passing through to exclude first character of comment,
                 // because state changes of lexer_continue in the loop can't
                 // affect the value of c in the curr loop

eof_err:
    FRONTEND_THROW_TRACED_ERR_WITH_POS(
        l->region->filename, l->line_view, l->line + 1, l->col, 2,
        "expected delimiter for comment starting on line %zu, col %zu. found "
        "EOF instead.",
        l->line + 1, l->col);

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
lexeme_t *lexer_lexify_token(lexer_t *l, lexeme_type_t type, size_t len);

/*
 * initializes the lexer's state.
 */
lexer_t *lexer_init(src_t *source);

/*
 * lexically analyzes all strings and adds them as lexemes to their respective
 * regions.
 */
bool lexer_lex_string(lexer_t *l, char c);

/*
 * lexically analyzes all numbers and adds them as lexemes to their respective
 * regions. this doesn't distincuate between integers, floats, negatives, hexes,
 * and so on. that's the job of the parser.
 */
bool lexer_lex_number(lexer_t *l, char c);

/*
 * visualizes all lexemes in all regions in format `TYPE(VALUE)`.
 * */
void lexer_visualize(lexer_t *l);

/*
 * the lexer routine - this step in the pipeline converts all non-whitespace
 * tokens into 'lexemes'. you can think about lexemes as all tokens generalized
 * into the smallest categories possible without losing meaning.
 */
void lexer(arena_t *trans_arena, src_t *source);
