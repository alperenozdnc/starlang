#include <starlang/replacer.h>

#include <stdbool.h>
#include <string.h>

nmspc_module_t *replacer_build_namespace_and_module(arena_t *arena,
                                                    const char *rhs,
                                                    size_t rhs_len) {
    size_t max_ident_size = rhs_len + 1; // +1 for \0

    nmspc_module_t *info = arena_alloc(arena, sizeof(*info));
    char *namespace = arena_alloc(arena, max_ident_size);
    char *module = arena_alloc(arena, max_ident_size);

    memset(info, 0, sizeof(*info));
    memset(namespace, 0, max_ident_size);
    memset(module, 0, max_ident_size);

    bool did_get_namespace = false;
    size_t j = 0;

    // accounting for semicolon with - 1
    for (size_t i = 0; i < rhs_len - 1; i++) {
        char c = rhs[i];

        if (c == ' ' || c == '\t') {
            continue;
        }

        if (!did_get_namespace && c == '>') {
            did_get_namespace = true;
            namespace[j] = '\0';

            j = 0;

            continue;
        }

        if (!did_get_namespace) {
            namespace[j++] = c;

            continue;
        }

        if (c == ';') {
            module[j] = '\0';

            continue;
        }

        module[j++] = c;
    }

    info->namespace = namespace;
    info->module = module;

    return info;
}
