#include <starlang/arena.h>
#include <starlang/def.h>
#include <starlang/frontend.h>
#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void replacer(const char *content, size_t len) {
    arena_t *replacer_arena = arena_init(len);

    nmspc_decl_t **decl = replacer_get_namespace_decl(replacer_arena);
    nmspc_node_t *gnt = replacer_init_gnt(replacer_arena, "main.st");

    replacer_compile_gnt(replacer_arena, decl, gnt, content, len);
    replacer_visualize_gnt(gnt);

    replacer_print_ref_gnt();
    arena_free(replacer_arena);
}
