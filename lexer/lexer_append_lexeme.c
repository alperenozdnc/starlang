#include <starlang/arena.h>
#include <starlang/lexer.h>

#include <string.h>

void lexer_append_lexeme(arena_t *arena, lexer_region_t *region,
                         lexeme_t **tail) {
    if (region->lexemes == NULL) {
        region->lexemes = arena_alloc(arena, sizeof(*region->lexemes));
        *tail = region->lexemes;
    } else {
        (*tail)->next = arena_alloc(arena, sizeof(*region->lexemes));
        *tail = (*tail)->next;
    }

    memset(*tail, 0, sizeof(**tail));
}
