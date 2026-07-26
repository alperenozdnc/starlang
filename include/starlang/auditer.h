#pragma once

#include <starlang/transitions.h>

#include <stdbool.h>

/* the auditer is a no-modify single pass 'checker', and that's why it has
 no inherent data structure (like lexer_t for the lexer), it only reads
 the lexical info and either allows or stops compilation */

/*
 * audits given lexical information for correct structure and obvious mistakes,
 * to decide if given lexemes are suitable to go on to be parsed. although this
 * doesn't cover the whole surface area of possible errors, it only leaves
 * errors that are detectable while building the AST in the parser.
 *
 * returns `true` if there are any errors, and `false` otherwise.
 */
bool auditer(lexical_info_t *info);
