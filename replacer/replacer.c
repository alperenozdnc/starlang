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
    arena_t *replacer_arena = arena_init(4096);
    size_t cursor = 0;

    size_t line_count = 0;
    char *line = NULL;

    size_t import_action_len = strlen(REPLACER_IMPORT_ACTION);

    size_t action_size = import_action_len + 1;
    char action_buf[action_size];
    memset(action_buf, 0, action_size);

    while ((line = util_read_line(replacer_arena, content, len, &cursor)) !=
           NULL) {
        line_count++;

        char *action_start = strchr(line, '@');

        bool is_empty = line[0] == '\0';
        bool is_comment =
            line[0] == '#' || util_does_str_end_with_suffix(line, "##");
        bool isnt_action = !action_start;

        if (is_empty || is_comment || isnt_action)
            continue;

        size_t line_len = strlen(line);

        size_t action_idx = 0;
        size_t line_idx = 0;

        bool seen_action_start = false;

        while (action_idx < import_action_len) {
            if (action_idx > line_len - 1)
                break;

            if (line[line_idx] == ' ' || line[line_idx] == '\t') {
                if (seen_action_start) {
                    frontend_prettyprint_errpos(line, line_count, line_idx + 1,
                                                1);

                    FRONTEND_PRINT(FRONTEND_ERR, FRONTEND_PRINT_PREFIX,
                                   "unallowed space at line %zu, col %zu.",
                                   line_count, line_idx + 1);

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
            frontend_prettyprint_errpos(line, line_count, col,
                                        import_action_len);

            FRONTEND_PRINT(FRONTEND_ERR, FRONTEND_PRINT_PREFIX,
                           "unrecognized action '%s' at line %zu, col %zu.",
                           action_buf, line_count, col);

            exit(EXIT_FAILURE);
        }

        if (line[line_idx] != ' ') {
            frontend_prettyprint_errpos(line, line_count, line_idx + 1, 1);

            FRONTEND_PRINT(FRONTEND_ERR, FRONTEND_PRINT_PREFIX,
                           "unrecognized character '%c' at line %zu, col %zu. "
                           "expected ' '.",
                           line[line_idx], line_count, line_idx + 1);

            exit(EXIT_FAILURE);
        }

        printf("import line found -> %s\n", line);

        memset(action_buf, 0, import_action_len + 1);
    }

    arena_free(replacer_arena);

    return;
}
