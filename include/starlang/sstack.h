#pragma once

#include <starlang/lexemes.h>

#include <stdbool.h>
#include <stddef.h>

/*
 * the symbol struct. it holds the character of the symbol, its line and and
 * column, and its predecessor in the stack.
 */
typedef struct symbol_t {
    char c;
    size_t line;
    size_t col;

    struct symbol_t *below;
} symbol_t;

/*
 * the symbol stack struct. it holds the topmost and bottommost symbol in the
 * symbol stack. these two data points can be used for length and emptiness
 * calculations.
 */
typedef struct {
    symbol_t *top;
    symbol_t *bottom;
} symbol_stack_t;

/*
 * maps a character to a lexeme.
 */
char sstack_map(lexeme_type_t lex);

/*
 * pushes a symbol to the top of a symbol stack.
 */
void sstack_push(symbol_stack_t *st, symbol_t *sym);

/*
 * removes the topmost symbol of a symbol stack.
 */
void sstack_pop(symbol_stack_t *st);

/*
 * returns the topmost symbol of a symbol stack.
 */
symbol_t *sstack_peek(symbol_stack_t *st);

/*
 * returns a boolean value for whether a symbol stack has no symbols.
 */
bool sstack_is_empty(symbol_stack_t *st);
