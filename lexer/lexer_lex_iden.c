#include <starlang/frontend.h>
#include <starlang/lexer.h>

#include <ctype.h>

bool lexer_lex_iden(lexer_t *l, char c) {
    bool wrong_mode_for_num =
        (l->mode != LEX_MODE_NORMAL && l->mode != LEX_MODE_IDEN);

    if (wrong_mode_for_num)
        return false;

    bool is_letter = isalpha((unsigned char)c);
    bool is_num = isdigit((unsigned char)c);
    bool is_separator = (c == '_');

    bool is_delim = (is_letter || is_separator);
    bool could_belong_to_iden = is_delim || is_num;

    lexeme_t *lex = NULL;

    if (l->mode == LEX_MODE_NORMAL) {
        if (!is_delim)
            return false;

        l->mode = LEX_MODE_IDEN;
        lex = lexer_lexify_token(l, LEX_IDENT, 1);

        lex->col = l->col - 1;
        lex->view = l->src + l->pos - 1;

        return true;
    }

    lex = l->region->tail_lexeme;
    assert(lex != NULL);

    bool is_last_char_in_line = (l->col - 1 == lex->line_view_len);

    if (l->mode == LEX_MODE_IDEN) {
        if (could_belong_to_iden) {
            if (is_last_char_in_line)
                l->mode = LEX_MODE_NORMAL;

            lex->view_len++;

            return true;
        }

        l->mode = LEX_MODE_NORMAL;

        return false;
    }

    __builtin_unreachable();
}
