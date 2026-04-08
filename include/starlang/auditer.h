#pragma once

#include <starlang/transitions.h>

#include <stdbool.h>

/*
 * audits given lexical information for correct structure and obvious mistakes,
 * to decide if given lexemes are suitable to go on to be parsed. although this
 * doesn't cover the whole surface area of possible errors, it only leaves
 * errors that are detectable while building the AST in the parser.
 *
 * returns `true` if there are any errors, and `false` otherwise.
 */
bool auditer(lexical_info_t *info);
