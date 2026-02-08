#include <starlang/frontend.h>
#include <starlang/lexer.h>

#define DBQUOT '"'
#define QUOT '\''

bool lexer_lex_string(lexer_t *l, char c) {
    bool char_cant_belong_to_str =
        (l->mode != LEX_MODE_NORMAL && l->mode != LEX_MODE_STR);

    if (char_cant_belong_to_str)
        return false;

    bool is_last_char = (l->pos == l->src_len - 1);
    bool is_delimiter = (c == DBQUOT || c == QUOT);
    bool str_begin = (l->mode == LEX_MODE_NORMAL && is_delimiter);

    lexeme_t *lex;

    if (str_begin) {
        l->mode = LEX_MODE_STR;
        lex = lexer_lexify_token(l, LEX_STRING, 0);

        lex->col = l->col;

        if (is_last_char)
            goto eof_err;

        lex->view = l->src + l->pos;

        l->is_str_dbquot = (c == DBQUOT);

        return true;
    }

    lex = l->region->tail_lexeme;

    bool str_continue = (l->mode == LEX_MODE_STR && !is_delimiter);

    if (str_continue) {
        if (is_last_char)
            goto eof_err;

        lex->view_len++;

        return true;
    }

    bool str_seen_delim = l->mode == LEX_MODE_STR && is_delimiter;

    if (str_seen_delim) {
        bool delim_is_escaped = false;

        size_t backslash_count = 0;

        // in the -2, -1 is for making the col zero-based, and -1 is for
        // starting at the first char before delim
        for (int i = l->col - 2; i >= 0; i--) {
            char c = lex->line_view[i];

            if (c != '\\')
                break;

            backslash_count++;
        }

        delim_is_escaped = (backslash_count % 2 != 0);

        bool not_matching_delim = (l->is_str_dbquot && c != DBQUOT) ||
                                  (!l->is_str_dbquot && c == DBQUOT);

        if (delim_is_escaped || not_matching_delim) {
            lex->view_len += 1;

            return true;
        }

        l->mode = LEX_MODE_NORMAL;

        return true;
    }

    // this is redundant because all predicates in str_passive have already been
    // verified by the if guards above, but this lets me assert unreachability
    // after this statement which may be an optimization
    bool str_passive = (!char_cant_belong_to_str && !str_begin &&
                        !str_seen_delim && !str_continue);
    if (str_passive)
        return false;

    __builtin_unreachable();

eof_err:
    FRONTEND_THROW_TRACED_ERR_WITH_POS(
        l->region->filename,
        util_dup_slice(l->arena, lex->line_view, lex->line_view_len),
        lex->line + 1, lex->col, 1,
        "found EOF instead of termination while reading string on line %zu, "
        "col %zu.",
        lex->line + 1, lex->col);
}
