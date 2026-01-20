#include <starlang/arena.h>
#include <starlang/replacer.h>

#include <stdio.h>

void replacer(const char *content, size_t len) {
    arena_t *replacer_arena = arena_init(len);

    nmspc_decl_t **decl = replacer_get_nmspc_decl(replacer_arena);
    nmspc_link_t *gnt = replacer_init_gnt(replacer_arena, "main.st");

    // first gnt as root / second as parent information
    replacer_compile_gnt(replacer_arena, decl, gnt, gnt, content, len);
    replacer_visualize_gnt(gnt);

    size_t link_count = replacer_get_gnt_link_count(gnt);

    nmspc_link_t **gnt_flat =
        replacer_flatten_gnt(replacer_arena, gnt, link_count);

    replacer_print_flattened_gnt(gnt_flat, link_count);

    arena_free(replacer_arena);
}
