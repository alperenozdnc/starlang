#include <starlang/lexer.h>

char lexer_continue(lexer_t *l) {
    if (l->pos == 0)
        lexer_get_line(l);

    if (l->pos == l->src_len)
        return EOF;

    char c = l->src[l->pos++];

    if (c != '\n') {
        l->col++;

        return c;
    }

    l->line++;
    l->col = 0;

    lexer_get_line(l);
    lexer_check_region(l);

    return c;
}
