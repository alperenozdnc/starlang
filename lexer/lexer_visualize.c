#include <starlang/lexer.h>
#include <starlang/utils.h>

void lexer_visualize(lexer_t *l) {
    lexer_region_t *reg = l->base_region;

    while (reg != NULL) {
        lexeme_t *lex = reg->lexemes;

        while (lex != NULL) {
            char *t = lexeme_to_str(lex->type);
            char *s = util_dup_slice(l->arena, lex->view, lex->view_len);

            printf("%s(%s)\n", t, s);

            lex = lex->next;
        }

        reg = reg->next;
    }
}
