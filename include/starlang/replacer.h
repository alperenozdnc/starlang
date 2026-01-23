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
} nmspc_node_t;

/* struct for holding a namespace node as a link in a linked list. */
typedef struct nmspc_link_t {
    nmspc_node_t *self;

    struct nmspc_link_t *next;

    struct nmspc_link_t *children;
    struct nmspc_link_t *tail;
} nmspc_link_t;

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
nmspc_module_t *replacer_build_nmspc_and_module(arena_t *arena, const char *rhs,
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
 * compares all nodes in a - usually incomplete - GNT (generated namespace tree)
 * to the given dependency info (namespace and module) and returns a boolean
 * value based on if it doesn't exist anywhere in the GNT.
 */
bool replacer_is_dep_uniq(nmspc_link_t *head, nmspc_module_t *dep_info);

/*
 * enforces that a dependency doesn't import itself in it's own dependency graph
 * by comparing the currently compiled namespace and module information against
 * the parent's.
 */
void replacer_enforce_no_self_import(char *line, size_t line_idx,
                                     size_t line_len, nmspc_link_t *parent,
                                     nmspc_module_t *dep_info);

/*
 * appends a child link into its parent link. this helper assumes that the link
 * to be appended is verified to be unique and non-cyclic.
 */
void replacer_append_child_to_link(nmspc_link_t *parent, nmspc_link_t *child);

/*
 * this rough helper visualizes a generated namespace tree. see comments for
 * `nmspc_node_t` for more information. depth should be passed as 0 if not
 * recursing.
 */
void _replacer_visualize_gnt(nmspc_link_t *root, size_t depth);
#define replacer_visualize_gnt(root) _replacer_visualize_gnt(root, 0)

/*
 * reads all namespace declarations from the namespace file (`.starnmspc`). this
 * is used to resolve `@import` statements at the filesystem.
 */
nmspc_decl_t **replacer_get_nmspc_decl(arena_t *arena, const char *parent_path);

/*
 * initializes a namespace link with the given values.
 */
nmspc_link_t *replacer_init_nmspc_link(arena_t *arena, nmspc_node_t *parent,
                                       const char *path, const char *namespace,
                                       const char *module);

/*
 * this builds a node that belongs to a GNT (generated namespace tree) from
 * the given namespace and module names from a statement. it compares
 * against the declarations present in `**declarations` and verifies
 * filesystem existence.
 */
nmspc_link_t *replacer_get_nmspc_link(arena_t *arena, const char *parent_path,
                                      nmspc_node_t *parent_node,
                                      nmspc_decl_t **declarations,
                                      size_t line_idx, size_t line_len,
                                      const char *line, const char *namespace,
                                      const char *module);

/*
 * generates the root node for a GNT (generated namespace tree). `module` is
 * the file that the interpreter was called on.
 */
nmspc_link_t *replacer_init_gnt(arena_t *arena, const char *module);

/*
 * generates a GNT (generated namespace tree) by recursively compiling
 * import dependencies of module. each branch ends at the module that
 * imports to other modules.
 */
void replacer_compile_gnt(arena_t *arena, const char *parent_path,
                          nmspc_decl_t **decl, nmspc_link_t *root,
                          nmspc_link_t *parent, const char *content,
                          size_t content_len);

/*
 * counts every link inside of a GNT (generated namespace tree) recursively.
 * since this takes `link_count` with an initial value of 0 which must not
 * be in a variable yet, the `replacer_get_gnt_link_count` macro should be
 * used instead for clearer code intent.
 */
size_t _replacer_get_gnt_link_count(nmspc_link_t *parent, size_t link_count);

#define replacer_get_gnt_link_count(gnt) _replacer_get_gnt_link_count(gnt, 0)

/*
 * flattens a GNT (generated namespace tree) into a set of links in a manner
 * where every dependency has its sub-dependencies in scope.
 */
nmspc_link_t **replacer_flatten_gnt(arena_t *arena, nmspc_link_t *gnt,
                                    size_t link_count);

/*
 * prints a flattened GNT (generated namespace tree).
 */
void replacer_print_flattened_gnt(nmspc_link_t **gnt_flat, size_t link_count);

/*
 * the replacer routine - resolves namespaces and imports. the name comes
 * because this is essentially doing text replacement for the imports, but
 * not exactly.
 */
void replacer(const char *parent_path, const char *filename,
              const char *content, size_t len);
