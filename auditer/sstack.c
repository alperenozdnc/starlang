#include <starlang/lexemes.h>
#include <starlang/sstack.h>

static lexeme_type_t SYM_LEXEMES[] = {LEX_LPAREN,   LEX_RPAREN,   LEX_LBRACKET,
                                      LEX_RBRACKET, LEX_LSQUIRLY, LEX_RSQUIRLY};

static char SYM_CHARS[] = {'(', ')', '[', ']', '{', '}'};

char sstack_map(lexeme_type_t lex) {
    size_t total = sizeof(SYM_LEXEMES) / sizeof(SYM_LEXEMES[0]);
    size_t i = 0;

    while (i + 1 <= total) {
        if (SYM_LEXEMES[i] == lex)
            return SYM_CHARS[i];

        i++;
    }

    return '\0';
}

void sstack_push(symbol_stack_t *st, symbol_t *sym) {
    if (st->bottom == NULL)
        st->bottom = sym;

    st->top = sym;
}

void sstack_pop(symbol_stack_t *st) {
    if (st->top == st->bottom) {
        st->top = NULL;
        st->bottom = NULL;

        return;
    }

    st->top = st->top->below;
}

symbol_t *sstack_peek(symbol_stack_t *st) {
    return st->top;
}

bool sstack_is_empty(symbol_stack_t *st) {
    return st->top == NULL;
}
