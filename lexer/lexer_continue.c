#include <starlang/lexer.h>

char lexer_continue(lexer_t *l) {
    if (l->pos == l->src_len)
        return EOF;

    char c = l->src[l->pos++];

    if (c != '\n') {
        l->col++;

        return c;
    }

    l->line++;
    l->col = 0;

    if (l->region->next && ((l->line + l->offset) > l->region->end)) {
        l->region = l->region->next;
        l->offset = l->region->start;
        l->line = 0;
    }

    return c;
}
