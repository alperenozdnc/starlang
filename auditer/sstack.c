#include <starlang/sstack.h>

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

extern inline symbol_t *sstack_peek(symbol_stack_t *st) {
    return st->top;
}

extern inline bool sstack_is_empty(symbol_stack_t *st) {
    return st->top == NULL;
}
