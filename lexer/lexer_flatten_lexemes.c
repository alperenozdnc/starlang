#include <starlang/arena.h>
#include <starlang/lexer.h>
#include <starlang/transitions.h>
#include <starlang/utils.h>

lexical_info_t *lexer_flatten_lexemes(arena_t *trans_arena, lexer_t *l) {
    lexical_info_t *info = arena_alloc(trans_arena, sizeof(*info));

    info->lexeme_count = 0;

    lexer_region_t *reg = l->base_region;

    while (reg != NULL) {
        lexeme_t *lex = reg->lexemes;

        while (lex != NULL) {
            info->lexeme_count++;

            lex = lex->next;
        }

        reg = reg->next;
    }

    info->lexemes =
        arena_alloc(trans_arena, info->lexeme_count * (sizeof(uintptr_t)));

    reg = l->base_region;

    size_t i = 0;

    while (reg != NULL) {
        lexeme_t *lex = reg->lexemes;

        while (lex != NULL) {
            lex_t *_lex = arena_alloc(trans_arena, sizeof(*_lex));

            memcpy(_lex, lex,
                   sizeof(*lex) -
                       8); // -8 is because lex_t has exactly 8 bytes less than
                           // lexeme_t, and all other fields are identical

            info->lexemes[i++] = _lex;

            lex = lex->next;
        }

        reg = reg->next;
    }

    return info;
}
