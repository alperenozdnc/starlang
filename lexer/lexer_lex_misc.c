#include <starlang/frontend.h>
#include <starlang/lexer.h>

#define SINGLECHAR_CASE(c, t)                                                  \
    case c:                                                                    \
        lex = lexer_lexify_token(l, t, 1);                                     \
        break;

#define MULTICHAR_CASE(t, len)                                                 \
    do {                                                                       \
        lex = lexer_lexify_token(l, t, len);                                   \
        CONSUME_CHARS((len - 1));                                              \
    } while (0);

#define CONSUME_CHARS(n)                                                       \
    l->mode = LEX_MODE_HIJACKED;                                               \
    do {                                                                       \
        lexer_continue(l);                                                     \
        skip_cnt++;                                                            \
    } while (skip_cnt < n);                                                    \
    l->mode = LEX_MODE_NORMAL;

bool lexer_lex_misc(lexer_t *l, char c) {
    lexeme_t *lex = NULL;

    char nc = lexer_peek(l, 0);
    char nnc = lexer_peek(l, 1);
    size_t skip_cnt = 0;

    switch (c) {
        SINGLECHAR_CASE(';', LEX_SEMI);
        SINGLECHAR_CASE(':', LEX_COLON);
        SINGLECHAR_CASE(',', LEX_COMMA);
        SINGLECHAR_CASE('~', LEX_BITWISE_NOT);
        SINGLECHAR_CASE('(', LEX_LPAREN);
        SINGLECHAR_CASE(')', LEX_RPAREN);
        SINGLECHAR_CASE('[', LEX_LBRACKET);
        SINGLECHAR_CASE(']', LEX_RBRACKET);
        SINGLECHAR_CASE('{', LEX_LSQUIRLY);
        SINGLECHAR_CASE('}', LEX_RSQUIRLY);
        SINGLECHAR_CASE('?', LEX_TERNARY);

        case '!':
            if (nc != '=') {
                lex = lexer_lexify_token(l, LEX_NOT, 1);
                break;
            }

            MULTICHAR_CASE(LEX_INEQUALITY_CHECK, 2);

            break;
        case '^':
            if (nc != '=') {
                lex = lexer_lexify_token(l, LEX_XOR, 1);
                break;
            }

            MULTICHAR_CASE(LEX_XOR_EQ, 2);

            break;
        case '=':
            if (nc != '=') {
                lex = lexer_lexify_token(l, LEX_EQ, 1);
                break;
            }

            MULTICHAR_CASE(LEX_EQUALITY_CHECK, 2);

            break;
        case '+':
            if (nc != '+' && nc != '=') {
                lex = lexer_lexify_token(l, LEX_PLUS, 1);
                break;
            }

            if (nc == '+') {
                MULTICHAR_CASE(LEX_INCREMENT, 2);
            } else if (nc == '=') {
                MULTICHAR_CASE(LEX_PLUS_EQUAL, 2);
            }

            break;
        case '-':
            if (nc != '-' && nc != '=') {
                lex = lexer_lexify_token(l, LEX_MINUS, 1);
                break;
            }

            if (nc == '-') {
                MULTICHAR_CASE(LEX_DECREMENT, 2);
            } else if (nc == '=') {
                MULTICHAR_CASE(LEX_MINUS_EQUAL, 2);
            }

            break;
        case '/':
            if (nc != '=') {
                lex = lexer_lexify_token(l, LEX_DIVIDE, 1);
                break;
            }

            MULTICHAR_CASE(LEX_DIVIDE_EQUAL, 2);

            break;
        case '*':
            if (nc != '*' && nc != '=') {
                lex = lexer_lexify_token(l, LEX_MULTIPLY, 1);
                break;
            }

            if (nc == '*') {
                MULTICHAR_CASE(LEX_EXPONENTIATE, 2);
            } else if (nc == '=') {
                MULTICHAR_CASE(LEX_MULTIPLY_EQUAL, 2);
            }

            break;
        case '<':
            if (nc != '<' && nc != '=') {
                lex = lexer_lexify_token(l, LEX_LESS_THAN, 1);
                break;
            }

            if (nc == '=') {
                MULTICHAR_CASE(LEX_LESS_THAN_OR_EQUAL_TO, 2);
            } else if (nc == '<') {
                if (nnc != '=') {
                    MULTICHAR_CASE(LEX_BITSHIFT_LEFT, 2);
                } else {
                    MULTICHAR_CASE(LEX_BITSHIFT_LEFT_EQ, 3);
                }
            }

            break;
        case '>':
            if (nc != '>' && nc != '=') {
                lex = lexer_lexify_token(l, LEX_GREATER_THAN, 1);
                break;
            }

            if (nc == '=') {
                MULTICHAR_CASE(LEX_GREATER_THAN_OR_EQUAL_TO, 2);
            } else if (nc == '>') {
                if (nnc != '=') {
                    MULTICHAR_CASE(LEX_BITSHIFT_RIGHT, 2);
                } else {
                    MULTICHAR_CASE(LEX_BITSHIFT_RIGHT_EQ, 3);
                }
            }

            break;
        case '%':
            if (nc != '=') {
                lex = lexer_lexify_token(l, LEX_MODULUS, 1);
                break;
            }

            MULTICHAR_CASE(LEX_MODULUS_EQUAL, 2);

            break;
        case '&':
            if (nc != '&' && nc != '=') {
                lex = lexer_lexify_token(l, LEX_BITWISE_AND, 1);
                break;
            }

            if (nc == '&') {
                MULTICHAR_CASE(LEX_LOGICAL_AND, 2);
            } else if (nc == '=') {
                MULTICHAR_CASE(LEX_BITWISE_AND_EQ, 2);
            }

            break;
        case '|':
            if (nc != '|' && nc != '=') {
                lex = lexer_lexify_token(l, LEX_BITWISE_OR, 1);
                break;
            }

            if (nc == '|') {
                MULTICHAR_CASE(LEX_LOGICAL_OR, 2);
            } else if (nc == '=') {
                MULTICHAR_CASE(LEX_BITWISE_OR_EQ, 2);
            }

            break;
    }

    if (lex != NULL) {
        lex->col = l->col - skip_cnt;
        lex->view = l->src + l->pos - 1 - skip_cnt;

        return true;
    }

    return false;
}
