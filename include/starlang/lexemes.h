#pragma once

#define GET_LEXEMES()                                                          \
    X(LEX_EOF)                                                                 \
    X(LEX_SEMI)                                                                \
    X(LEX_COLON)                                                               \
    X(LEX_COMMA)                                                               \
    X(LEX_COMMENT)                                                             \
    X(LEX_PERIOD)                                                              \
                                                                               \
    X(LEX_NOT)                                                                 \
    X(LEX_BITWISE_NOT)                                                         \
                                                                               \
    X(LEX_XOR)                                                                 \
                                                                               \
    X(LEX_ACCESS_MEMBER)                                                       \
                                                                               \
    X(LEX_LPAREN)                                                              \
    X(LEX_RPAREN)                                                              \
    X(LEX_LBRACKET)                                                            \
    X(LEX_RBRACKET)                                                            \
    X(LEX_LSQUIRLY)                                                            \
    X(LEX_RSQUIRLY)                                                            \
                                                                               \
    X(LEX_DQUOTE)                                                              \
    X(LEX_QUOTE)                                                               \
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
                                                                               \
    X(LEX_LOGICAL_OR)                                                          \
    X(LEX_BITWISE_OR)                                                          \
                                                                               \
    X(LEX_TERNARY)                                                             \
                                                                               \
    X(LEX_IDENT)                                                               \
    X(LEX_NUM)                                                                 \
    X(LEX_STRING)
