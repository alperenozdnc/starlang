/*
 * this header contains all data structures passed between pipeline steps, and
 * don't really belong fully to any step on their own. for example, src_t is
 * passed from the replacer to the lexer.
 */

#pragma once

#include <starlang/lexemes.h>

#include <stddef.h>

/* struct holding information about a file's position in the source. this is
 * purely for error printing as the lexer won't need to know which files are
 * which thanks to the flattened source. */
typedef struct {
    size_t start;
    size_t end;

    char *file_path;
} file_range_t;

/* monolithic structure holding all information about the source of the program
 * to be passed onto the lexer. after this is passed and memcpy'd, the replacer
 * arena can be freed. */
typedef struct {
    char *content;
    size_t *import_indices;
    file_range_t **file_ranges;

    size_t content_len;
    size_t import_indices_len;
    size_t file_ranges_len;
} src_t;

typedef struct {
    LEXEME_FIELDS()
} lex_t;

typedef struct {
    lex_t **lexemes;
    size_t lexeme_count;
} lexical_info_t;
