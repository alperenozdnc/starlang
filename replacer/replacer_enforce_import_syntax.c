#include <starlang/def.h>
#include <starlang/frontend.h>
#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void replacer_enforce_import_syntax(char *line, size_t lines_size,
                                    size_t line_len, char *rhs, size_t rhs_len,
                                    size_t lhs_len) {
    size_t action_idx = 0;
    size_t line_idx = 0;

    size_t action_size = IMPORT_ACTION_LEN + 1;
    char action_buf[action_size];
    memset(action_buf, 0, action_size);

    bool seen_action_start = false;

    while (action_idx < IMPORT_ACTION_LEN) {
        if (action_idx > line_len - 1)
            break;

        if (line[line_idx] == ' ' || line[line_idx] == '\t') {
            if (seen_action_start) {
                size_t col = line_idx + 1;

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
        size_t col = line_idx - action_idx + 1;

        FRONTEND_THROW_ERR_WITH_POS(
            line, lines_size, col, IMPORT_ACTION_LEN,
            "unrecognized action '%s' at line %zu, col %zu.", action_buf,
            lines_size, col);
    }

    if (line[line_idx] != ' ') {
        size_t col = line_idx + 1;

        FRONTEND_THROW_ERR_WITH_POS(
            line, lines_size, col, 1,
            "unrecognized character '%c' at line %zu, col %zu. "
            "expected ' '.",
            line[line_idx], lines_size, col);
    }

    size_t len_before_indicator = strcspn(rhs, ">");
    size_t len_before_semicolon = strcspn(rhs, ";");

    if (len_before_indicator == rhs_len) {
        FRONTEND_THROW_ERR_WITH_POS((char *)line, line_idx, 1, line_len,
                                    "no '>' symbol found to distinctuate "
                                    "between namespace and module at line %zu.",
                                    line_idx);
    }

    if (len_before_semicolon == rhs_len) {
        FRONTEND_THROW_ERR_WITH_POS(
            (char *)line, line_idx, line_len, 1,
            "missing semicolon after line %zu, col %zu.", line_idx, line_len);
    }

    if (!util_does_str_end_with_suffix(rhs, STARLANG_FILE_EXT ";")) {
        size_t start = lhs_len + 1; // +1 to skip space after lhs
        size_t len = rhs_len - 1;   // -1 to not include semicolon at end

        FRONTEND_THROW_ERR_WITH_POS((char *)line, line_idx, start, len,
                                    "all modules must have '.st' as their "
                                    "extension. discrepancy at line %zu.",
                                    line_idx);
    }
}
