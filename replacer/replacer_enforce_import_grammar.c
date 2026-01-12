#include <starlang/frontend.h>
#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <stdlib.h>
#include <string.h>

void replacer_enforce_import_grammar(const char *line, size_t line_idx,
                                     const char *namespace, const char *module,
                                     size_t lhs_len) {
    size_t namespace_len = strlen(namespace);
    size_t module_len = strlen(module);

    char offending_namespace_char =
        util_enforce_str_rules(namespace, namespace_len, CHAR_ALPHA, "_");
    char offending_module_char =
        util_enforce_str_rules(module, module_len, CHAR_ALPHA, "_.");

    if (offending_namespace_char) {
        size_t i = strchr(namespace, offending_namespace_char) - namespace;
        size_t col = lhs_len + i + 1; // +1 for making it 1-based

        FRONTEND_THROW_ERR_WITH_POS(
            (char *)line, line_idx, col, 1,
            "only alphabetical letters and underscores are allowed in "
            "namespace names.\ndiscrepancy found after symbol '%c' at line "
            "%zu, col %zu.",
            offending_namespace_char, line_idx, col);
    }

    if (offending_module_char) {
        size_t i = strchr(module, offending_module_char) - module;
        size_t col = lhs_len + namespace_len + i +
                     2; // +1 for skipping > and +1 for making it 1-based

        FRONTEND_THROW_ERR_WITH_POS(
            (char *)line, line_idx, col, 1,
            "only alphabetical letters, underscores and periods are "
            "allowed in module names.\ndiscrepancy found after symbol '%c' "
            "at "
            "line %zu, col %zu.",
            offending_module_char, line_idx, col);
    }
}
