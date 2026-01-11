#include <starlang/arena.h>
#include <starlang/def.h>
#include <starlang/frontend.h>
#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// detects @import statements, throws on improper syntax -> done
// collects names of dependencies imports ask for, throws on improper syntax ->
// verifies imported namespaces exist in the .starnmspc file ->
// verifies declared namespaces exist on disk ->
// verifies imported files exists on namespace ->
// loads namespaces and redoes the process if they have any imports ->
// globally keeping track of all imports and making sure not to reimport
// anything in scope, then finally writes to a final string after all
// dependencies have been stripped down to their root (the file you get after
// going import by import into a file that has no imports) ->
// also makes sure no line/col/filename information is lost after passing the
// final output string to the lexer by also passing 'file start offsets' ->

void replacer(const char *content, size_t len) {
    arena_t *replacer_arena = arena_init(4096);

    size_t cursor_pos = 0;
    size_t lines_size = 0;
    size_t line_len = 0;
    char *line = NULL;

    size_t import_action_len = strlen(REPLACER_IMPORT_ACTION);

    size_t action_size = import_action_len + 1;
    char action_buf[action_size];
    memset(action_buf, 0, action_size);

    while ((line = util_read_line(replacer_arena, content, len, &line_len,
                                  &lines_size, &cursor_pos)) != NULL) {
        char *action_start = strchr(line, '@');

        bool is_empty = line_len == 0;
        bool is_comment =
            line[0] == '#' || util_does_str_end_with_suffix(line, "##");
        bool isnt_action = !action_start;

        if (is_empty || is_comment || isnt_action)
            continue;

        size_t action_idx = 0;
        size_t line_idx = 0;

        bool seen_action_start = false;

        while (action_idx < import_action_len) {
            if (action_idx > line_len - 1)
                break;

            if (line[line_idx] == ' ' || line[line_idx] == '\t') {
                if (seen_action_start) {
                    frontend_prettyprint_errpos(line, lines_size, line_idx + 1,
                                                1);

                    FRONTEND_PRINT(FRONTEND_ERR, FRONTEND_PRINT_PREFIX,
                                   "unallowed space at line %zu, col %zu.",
                                   lines_size, line_idx + 1);

                    exit(EXIT_FAILURE);
                }

                line_idx++;
                continue;
            }

            seen_action_start = true;

            action_buf[action_idx++] = line[line_idx++];
        }

        action_buf[action_idx] = '\0';

        size_t col = line_idx - action_idx + 1;

        if (strcmp(action_buf, REPLACER_IMPORT_ACTION) != 0) {
            frontend_prettyprint_errpos(line, lines_size, col,
                                        import_action_len);

            FRONTEND_PRINT(FRONTEND_ERR, FRONTEND_PRINT_PREFIX,
                           "unrecognized action '%s' at line %zu, col %zu.",
                           action_buf, lines_size, col);

            exit(EXIT_FAILURE);
        }

        if (line[line_idx] != ' ') {
            frontend_prettyprint_errpos(line, lines_size, line_idx + 1, 1);

            FRONTEND_PRINT(FRONTEND_ERR, FRONTEND_PRINT_PREFIX,
                           "unrecognized character '%c' at line %zu, col %zu. "
                           "expected ' '.",
                           line[line_idx], lines_size, line_idx + 1);

            exit(EXIT_FAILURE);
        }

        printf("import line found -> %s\n", line);

        memset(action_buf, 0, import_action_len + 1);
    }

    arena_free(replacer_arena);

    return;
}
