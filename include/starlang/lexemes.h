#pragma once

#include <stddef.h>

#define GET_LEXEMES()                                                          \
    X(LEX_EOF)                                                                 \
    X(LEX_EOL)                                                                 \
    X(LEX_SEMI)                                                                \
    X(LEX_COLON)                                                               \
    X(LEX_COMMA)                                                               \
    X(LEX_PERIOD)                                                              \
                                                                               \
    X(LEX_NOT)                                                                 \
    X(LEX_BITWISE_NOT)                                                         \
                                                                               \
    X(LEX_XOR)                                                                 \
    X(LEX_XOR_EQ)                                                              \
                                                                               \
    X(LEX_LPAREN)                                                              \
    X(LEX_RPAREN)                                                              \
    X(LEX_LBRACKET)                                                            \
    X(LEX_RBRACKET)                                                            \
    X(LEX_LSQUIRLY)                                                            \
    X(LEX_RSQUIRLY)                                                            \
                                                                               \
    X(LEX_EQ)                                                                  \
    X(LEX_INEQUALITY_CHECK)                                                    \
    X(LEX_EQUALITY_CHECK)                                                      \
                                                                               \
    X(LEX_PLUS)                                                                \
    X(LEX_PLUS_EQUAL)                                                          \
    X(LEX_INCREMENT)                                                           \
                                                                               \
    X(LEX_MINUS)                                                               \
    X(LEX_MINUS_EQUAL)                                                         \
    X(LEX_DECREMENT)                                                           \
                                                                               \
    X(LEX_DIVIDE)                                                              \
    X(LEX_DIVIDE_EQUAL)                                                        \
                                                                               \
    X(LEX_MULTIPLY)                                                            \
    X(LEX_MULTIPLY_EQUAL)                                                      \
    X(LEX_EXPONENTIATE)                                                        \
                                                                               \
    X(LEX_LESS_THAN)                                                           \
    X(LEX_LESS_THAN_OR_EQUAL_TO)                                               \
                                                                               \
    X(LEX_MODULUS)                                                             \
    X(LEX_MODULUS_EQUAL)                                                       \
                                                                               \
    X(LEX_GREATER_THAN)                                                        \
    X(LEX_GREATER_THAN_OR_EQUAL_TO)                                            \
                                                                               \
    X(LEX_BITSHIFT_LEFT)                                                       \
    X(LEX_BITSHIFT_LEFT_EQ)                                                    \
    X(LEX_BITSHIFT_RIGHT)                                                      \
    X(LEX_BITSHIFT_RIGHT_EQ)                                                   \
                                                                               \
    X(LEX_LOGICAL_AND)                                                         \
    X(LEX_BITWISE_AND)                                                         \
    X(LEX_BITWISE_AND_EQ)                                                      \
                                                                               \
    X(LEX_LOGICAL_OR)                                                          \
    X(LEX_BITWISE_OR)                                                          \
    X(LEX_BITWISE_OR_EQ)                                                       \
                                                                               \
    X(LEX_TERNARY)                                                             \
                                                                               \
    X(LEX_IDENT)                                                               \
    X(LEX_NUM)                                                                 \
    X(LEX_STRING)

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

#define LEXEME_FIELDS()                                                        \
    lexeme_type_t type;                                                        \
    char *view;                                                                \
    size_t view_len;                                                           \
    char *line_view;                                                           \
    size_t line_view_len;                                                      \
    size_t line;                                                               \
    size_t col;                                                                \
    size_t pos;                                                                \
    char *filename;

/*
 * struct for holding information about a lexeme. this belongs to a region of
 * type `lexer_region_t`.
 */
typedef struct lexeme_t {
    LEXEME_FIELDS()

    struct lexeme_t *next;
} lexeme_t;
