#include <starlang/arena.h>
#include <starlang/replacer.h>

void replacer(const char *content, size_t len) {
    arena_t *replacer_arena = arena_init(len);

    nmspc_decl_t **decl = replacer_get_nmspc_decl(replacer_arena);
    nmspc_node_t *gnt = replacer_init_gnt(replacer_arena, "main.st");

    replacer_compile_gnt(replacer_arena, decl, gnt, content, len);
    replacer_visualize_gnt(gnt);

    replacer_print_ref_gnt();
    arena_free(replacer_arena);
}
