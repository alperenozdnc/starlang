#pragma once

#include <starlang/arena.h>

#include <stdbool.h>
#include <stddef.h>

#define REPLACER_ACTION_SYMBOL '@'
#define REPLACER_IMPORT_ACTION "@import"
#define IMPORT_ACTION_LEN strlen(REPLACER_IMPORT_ACTION)

/* struct for holding information about a namespace in the GNT (generated
 namespace tree, yes, i came up with that weird name) */
typedef struct nmspc_node_t {
    const char *namespace;
    const char *module;
    const char *path;

    struct nmspc_node_t *parent;
    struct nmspc_node_t **children;
    size_t children_count;
} nmspc_node_t;

/* struct for holding information about a statement's namespace and module
   dependencies. */
typedef struct {
    const char *namespace;
    const char *module;
} nmspc_module_t;

/* struct for holding information about a namespace declaration (written in
   .starnmspc) */
typedef struct {
    const char *name;
    const char *path;
} nmspc_decl_t;

/*
 * decides if a given line should be interpreted as an action, this validates as
 * much as it can without enforcing grammar which is not a lot, and that's the
 * reason why its called a heuristic. returns a boolean result based on success.
 * this should come right before syntax enforcement
 * (`replacer_enforce_import_syntax`)
 */
bool replacer_heuristic_is_action(char *line, size_t line_len);

/*
 * enforces syntax for an action statement. this checks the given statement only
 * against `@import` because that's the only action in starlang.
 */
void replacer_enforce_import_syntax(char *line, size_t lines_size,
                                    size_t line_len, char *rhs, size_t rhs_len,
                                    size_t lhs_len);
/*
 * builds the namespace and module into given buffers from a syntactically and
 * enforced statement. this is to be used after `replacer_enforce_import_syntax`
 * and right before `replacer_enforce_import_grammar`.
 */
nmspc_module_t *replacer_build_namespace_and_module(arena_t *arena,
                                                    const char *rhs,
                                                    size_t rhs_len);
/*
 * enforces grammar for the right hand side of an action statement
 * (namespace>module.st). this checks the statement for correct letter usage
 * (i.e. only alphabetical and underscore letters)
 */
void replacer_enforce_import_grammar(const char *line, size_t line_idx,
                                     const char *namespace, const char *module,
                                     size_t lhs_len);

/*
 * this rough helper visualizes a generated namespace tree. see comments for
 * `nmspc_node_t` for more information.
 */
void replacer_visualize_gnt(nmspc_node_t *root);

/*
 * this creates a mock GNT (generated namespace tree) and visualizes it.
 * this is to compare against the tree generated from reading `spec/main.st`
 * and is only a 'sanity' checker, which means its only temporary.
 */
void replacer_print_ref_gnt();

/*
 * reads all namespace declarations from the namespace file (`.starnmspc`). this
 * is used to resolve `@import` statements at the filesystem.
 */
nmspc_decl_t **replacer_get_namespace_decl(arena_t *arena);

/*
 * this builds a node that belongs to a GNT (generated namespace tree) from the
 * given namespace and module names from a statement. it compares against the
 * declarations present in `**declarations` and verifies filesystem existence.
 */
nmspc_node_t *replacer_get_nmspc_node(arena_t *arena, nmspc_node_t *parent_node,
                                      nmspc_decl_t **declarations,
                                      const char *namespace,
                                      const char *module);

/*
 * generates the root node for a GNT (generated namespace tree). `module` is the
 * file that the interpreter was called on.
 */
nmspc_node_t *replacer_init_gnt(arena_t *arena, const char *module);

/*
 * generates a GNT (generated namespace tree) by recursively compiling import
 * dependencies of module. each branch ends at the module that imports to other
 * modules. todo: deduplicate imports, use linked lists for children instead of
 * arrays
 */
void replacer_compile_gnt(arena_t *arena, nmspc_decl_t **decl,
                          nmspc_node_t *parent_node, const char *content,
                          size_t content_len);

/*
 * the replacer routine - resolves namespaces and imports. the name comes
 * because this is essentially doing text replacement for the imports, but
 * not exactly.
 */
void replacer(const char *content, size_t len);
