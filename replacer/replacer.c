#include <starlang/arena.h>
#include <starlang/replacer.h>
#include <starlang/transitions.h>

src_t *replacer(arena_t *intermediate_arena, char *main_module_path,
                char *parent_path, char *filename, char *content, size_t len) {
    arena_t *replacer_arena = arena_init(len);

    nmspc_decl_t **decl = replacer_get_nmspc_decl(replacer_arena, parent_path);
    nmspc_link_t *gnt = replacer_init_gnt(replacer_arena, content, len,
                                          main_module_path, filename);

    size_t link_count = 1; // the root counts too

    replacer_compile_gnt(replacer_arena, parent_path, decl, gnt, gnt, content,
                         len, &link_count);

    nmspc_link_t **gnt_flat =
        replacer_flatten_gnt(replacer_arena, gnt, link_count);

    src_t *src = arena_alloc(intermediate_arena, sizeof(*src));

    replacer_compile_src(src, intermediate_arena, gnt_flat, link_count);

    arena_free(replacer_arena);

    return src;
}
