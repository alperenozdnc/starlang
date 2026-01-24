#include <starlang/arena.h>
#include <starlang/replacer.h>

void replacer(const char *main_module_path, const char *parent_path,
              const char *filename, const char *content, size_t len) {
    arena_t *replacer_arena = arena_init(len);

    nmspc_decl_t **decl = replacer_get_nmspc_decl(replacer_arena, parent_path);
    nmspc_link_t *gnt =
        replacer_init_gnt(replacer_arena, main_module_path, filename);

    replacer_compile_gnt(replacer_arena, parent_path, decl, gnt, gnt, content,
                         len);
    replacer_visualize_gnt(gnt);

    size_t link_count = replacer_get_gnt_link_count(gnt);

    nmspc_link_t **gnt_flat =
        replacer_flatten_gnt(replacer_arena, gnt, link_count);

    replacer_print_flattened_gnt(gnt_flat, link_count);

    arena_free(replacer_arena);
}
