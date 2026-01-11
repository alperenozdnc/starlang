#pragma once

#include <stddef.h>

#define REPLACER_ACTION_SYMBOL '@'
#define REPLACER_IMPORT_ACTION "@import"

/*
 * the replacer routine - resolves namespaces and imports. the name comes
 * because this is essentially doing text replacement for the imports, but not
 * exactly.
 */
void replacer(const char *content, size_t len);
