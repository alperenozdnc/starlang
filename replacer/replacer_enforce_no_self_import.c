#include <starlang/frontend.h>
#include <starlang/replacer.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void replacer_enforce_no_self_import(char *line, size_t line_idx,
                                     size_t line_len, nmspc_link_t *parent,
                                     nmspc_module_t *dep_info) {
    bool does_import_self = false;

    if (strcmp(parent->self->namespace, dep_info->namespace) == 0) {
        if (strcmp(parent->self->module, dep_info->module) == 0) {
            does_import_self = true;
        }
    }

    if (!does_import_self)
        return;

    FRONTEND_THROW_ERR_WITH_POS(
        line, line_idx, 1, line_len - 1, // to exclude the ';' from diagnostics
        "dependency '%s>%s' tries to import itself on "
        "line %zu.",
        dep_info->namespace, dep_info->module, line_idx);
}
