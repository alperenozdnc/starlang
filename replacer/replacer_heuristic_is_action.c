#include <starlang/def.h>
#include <starlang/frontend.h>
#include <starlang/replacer.h>
#include <starlang/utils.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool replacer_heuristic_is_action(char *line, size_t line_len) {
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

    return true;
}
