#pragma once

#include <starlang/arena.h>
#include <starlang/transitions.h>

#include <stdbool.h>
#include <stddef.h>

#define REPLACER_ACTION_SYMBOL '@'
#define REPLACER_IMPORT_ACTION "@import"
#define IMPORT_ACTION_LEN strlen(REPLACER_IMPORT_ACTION)

/* struct for holding information about a namespace in the GNT (generated
 namespace tree, yes, i came up with that weird name) */
typedef struct nmspc_node_t {
    char *namespace;
    char *module;
    char *path;

    char *content;
    size_t content_len;
    size_t content_line_count;

    size_t *import_indices_heap;
    size_t import_indices_size;
    size_t import_indices_len;

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
    char *namespace;
    char *module;
} nmspc_module_t;

/* struct for holding information about a namespace declaration (written in
   .starnmspc) */
typedef struct {
    char *name;
    char *path;
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
void replacer_enforce_import_syntax(char *module_path, char *line,
                                    size_t lines_size, size_t line_len,
                                    char *rhs, size_t rhs_len, size_t lhs_len);
/*
 * builds the namespace and module into given buffers from a syntactically and
 * enforced statement. this is to be used after `replacer_enforce_import_syntax`
 * and right before `replacer_enforce_import_grammar`.
 */
nmspc_module_t *replacer_build_nmspc_and_module(arena_t *arena, char *rhs,
                                                size_t rhs_len);
/*
 * enforces grammar for the right hand side of an action statement
 * (namespace>module.st). this checks the statement for correct letter usage
 * (i.e. only alphabetical and underscore letters)
 */
void replacer_enforce_import_grammar(char *module_path, char *line,
                                     size_t line_idx, char *namespace,
                                     char *module, size_t lhs_len);

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
void replacer_enforce_no_self_import(char *module_path, char *line,
                                     size_t line_idx, size_t line_len,
                                     nmspc_link_t *parent,
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
nmspc_decl_t **replacer_get_nmspc_decl(arena_t *arena, char *parent_path);

/*
 * initializes a namespace link with the given values.
 */
nmspc_link_t *replacer_init_nmspc_link(arena_t *arena, nmspc_node_t *parent,
                                       char *path, char *namespace,
                                       char *module);

/*
 * this builds a node that belongs to a GNT (generated namespace tree) from
 * the given namespace and module names from a statement. it compares
 * against the declarations present in `**declarations` and verifies
 * filesystem existence.
 */
nmspc_link_t *replacer_get_nmspc_link(arena_t *arena, char *parent_path,
                                      nmspc_node_t *parent_node,
                                      nmspc_decl_t **declarations,
                                      size_t line_idx, size_t line_len,
                                      char *line, char *namespace,
                                      char *module);

/*
 * generates the root node for a GNT (generated namespace tree). `module` is
 * the file that the interpreter was called on.
 */
nmspc_link_t *replacer_init_gnt(arena_t *arena, char *content,
                                size_t content_len, char *path, char *module);

/*
 * generates a GNT (generated namespace tree) by recursively compiling
 * import dependencies of module. each branch ends at the module that
 * imports to other modules.
 */
void replacer_compile_gnt(arena_t *arena, char *parent_dir, nmspc_decl_t **decl,
                          nmspc_link_t *root, nmspc_link_t *parent,
                          char *content, size_t content_len,
                          size_t *link_count);

/*
 * flattens a GNT (generated namespace tree) into a set of links in a manner
 * where every dependency has its sub-dependencies in scope.
 */
nmspc_link_t **replacer_flatten_gnt(arena_t *arena, nmspc_link_t *gnt,
                                    size_t link_count);

/*
 * visualizes flattened GNT (generated namespace tree).
 */
void replacer_visualize_flat_gnt(nmspc_link_t **gnt_flat, size_t link_count);

/*
 * pushes an import idx to an array to keep track of which lines the lexer
 * should ignore since the replacer doesn't really mutate anything except
 * flatten source in the correct order.
 */
void replacer_push_import_idx(nmspc_node_t *node, size_t new_idx);

/*
 * normalizes all dependencies in a GNT (generated namespace tree) into a single
 * source string and also provides lines to ignore and file offset information
 * used for error printing. the result of this is to be returned from the
 * replacer, and copied in the lexer. the replacer arena holds no purpose after
 * this function.
 */
void replacer_compile_src(src_t *src, arena_t *intermediate_arena,
                          nmspc_link_t **gnt_flat, size_t link_count);

/*
 * the replacer routine - resolves namespaces and imports and returns a
 * flattened source, the blacklist information (the import lines which are
 * ignored in the lexer) and offset information (used for error printing).
 */
src_t *replacer(arena_t *intermediate_arena, char *main_module_path,
                char *parent_path, char *filename, char *content, size_t len);
