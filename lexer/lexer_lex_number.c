#include <starlang/frontend.h>
#include <starlang/lexer.h>

#include <ctype.h>

#define FLOAT_DELIM 'f'
#define NEG_DELIM '-'
#define DECIMAL_SEPARATOR '.'
#define VISUAL_SEPARATOR '_'

bool lexer_lex_number(lexer_t *l, char c) {
    bool wrong_mode_for_num =
        (l->mode != LEX_MODE_NORMAL && l->mode != LEX_MODE_NUMBER);

    if (wrong_mode_for_num)
        return false;

    bool is_num_delim = isdigit((unsigned char)c);
    bool is_float_delim = (c == FLOAT_DELIM);
    bool is_neg_delim = (c == NEG_DELIM);

    bool char_is_delim = (is_num_delim || is_neg_delim || is_float_delim);
    bool char_belongs_to_num =
        (char_is_delim) || (c == DECIMAL_SEPARATOR) || (c == VISUAL_SEPARATOR);

    lexeme_t *lex = NULL;

    if (l->mode == LEX_MODE_NORMAL) {
        if (!char_is_delim)
            return false;

        char nc = lexer_peek(l, 0);
        bool nc_is_digit = isdigit((unsigned char)nc);

        if (c == FLOAT_DELIM && (!nc_is_digit && nc != '.'))
            return false;

        if (c == NEG_DELIM && (!nc_is_digit && nc != FLOAT_DELIM))
            return false;

        l->mode = LEX_MODE_NUMBER;
        lex = lexer_lexify_token(l, LEX_NUM, 1);

        lex->col = l->col - 1;
        lex->view = l->src + l->pos - 1;

        return true;
    }

    lex = l->region->tail_lexeme;
    assert(lex != NULL);

    bool is_last_char_in_line = (l->col - 1 == lex->line_view_len);

    if (char_belongs_to_num)
        lex->view_len++;

    if (!char_belongs_to_num || is_last_char_in_line) {
        l->mode = LEX_MODE_NORMAL;

        return false;
    }

    return true;
}
