#include <starlang/arena.h>
#include <starlang/def.h>
#include <starlang/frontend.h>
#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MULTICHAR_SPAN_COL                                                     \
    line_idx - action_idx + 1; // if the offending characters are more than one

#define SINGLECHAR_SPAN_COL                                                    \
    line_idx + 1 // if there's a single offending character

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

bool is_import_line(char *line, size_t lines_size, size_t line_len) {
    size_t import_action_len = strlen(REPLACER_IMPORT_ACTION);

    size_t action_size = import_action_len + 1;
    char action_buf[action_size];
    memset(action_buf, 0, action_size);

    char *action_start = strchr(line, REPLACER_ACTION_SYMBOL);

    if (line_len == 0) {
        return false;
    }

    if (line[0] == STARLANG_COMMENT_SYMBOL ||
        util_does_str_end_with_suffix(line,
                                      STARLANG_MULTILINE_COMMENT_SYMBOL)) {
        return false;
    }

    if (!action_start) {
        return false;
    }

    size_t action_idx = 0;
    size_t line_idx = 0;

    bool seen_action_start = false;

    while (action_idx < import_action_len) {
        if (action_idx > line_len - 1)
            break;

        if (line[line_idx] == ' ' || line[line_idx] == '\t') {
            if (seen_action_start) {
                size_t col = SINGLECHAR_SPAN_COL;

                FRONTEND_THROW_ERR_WITH_POS(
                    line, lines_size, col, 1,
                    "unallowed space at line %zu, col %zu.", lines_size, col);
            }

            line_idx++;
            continue;
        }

        seen_action_start = true;
        action_buf[action_idx++] = line[line_idx++];
    }

    action_buf[action_idx] = '\0';

    if (strcmp(action_buf, REPLACER_IMPORT_ACTION) != 0) {
        size_t col = MULTICHAR_SPAN_COL;

        FRONTEND_THROW_ERR_WITH_POS(
            line, lines_size, col, import_action_len,
            "unrecognized action '%s' at line %zu, col %zu.", action_buf,
            lines_size, col);
    }

    if (line[line_idx] != ' ') {
        size_t col = SINGLECHAR_SPAN_COL;

        FRONTEND_THROW_ERR_WITH_POS(
            line, lines_size, col, 1,
            "unrecognized character '%c' at line %zu, col %zu. "
            "expected ' '.",
            line[line_idx], lines_size, col);
    }

    return true;
}

void replacer(const char *content, size_t len) {
    arena_t *replacer_arena = arena_init(len);

    size_t cursor_pos = 0;
    size_t lines_size = 0;
    size_t line_len = 0;
    char *line = NULL;

    while ((line = util_read_line(replacer_arena, content, len, &line_len,
                                  &lines_size, &cursor_pos)) != NULL) {
        if (!is_import_line(line, lines_size, line_len))
            continue;

        printf("import line found -> %s\n", line);
    }

    arena_free(replacer_arena);
}
