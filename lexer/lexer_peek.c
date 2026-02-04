#include <starlang/lexer.h>

inline char lexer_peek(lexer_t *l, size_t offset) {
    return l->pos + offset < l->src_len ? l->src[l->pos + offset] : '\0';
}
