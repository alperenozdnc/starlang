#include <starlang/arena.h>
#include <starlang/replacer.h>

#include <stdio.h>

void replacer_print_ref_gnt() {
    arena_t *tree_arena = arena_init(1024);

    nmspc_node_t *root = arena_alloc(tree_arena, sizeof(*root));
    nmspc_node_t *io = arena_alloc(tree_arena, sizeof(*io));
    nmspc_node_t *module = arena_alloc(tree_arena, sizeof(*module));

    io->namespace = "starlang";
    io->module = "io.st";
    io->path = "starlang/io.st";
    io->children = arena_alloc(tree_arena, sizeof(void *));
    io->children[0] = NULL;
    io->parent = root;

    module->namespace = "namespace";
    module->module = "module.st";
    module->path = "namespace/module.st";
    module->children = arena_alloc(tree_arena, sizeof(void *) * 2);

    module->children[0] = io;
    module->children[1] = NULL;
    module->parent = root;

    root->namespace = "(root)";
    root->module = "main.st";
    root->path = "(root)/main.st";
    root->children = arena_alloc(tree_arena, sizeof(void *) * 3);
    root->children[0] = io;
    root->children[1] = module;
    root->children[2] = NULL;
    root->parent = NULL;

    printf("\n-----REFERENCE-------\n\n");
    replacer_visualize_gnt(root, 0);
    printf("\n");

    arena_free(tree_arena);
}
