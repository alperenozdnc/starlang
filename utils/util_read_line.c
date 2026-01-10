#include <starlang/arena.h>
#include <starlang/utils.h>

#include <string.h>

char *util_read_line(arena_t *arena, const char *buf, size_t buf_len,
                     size_t *start_idx) {
    if ((*start_idx + 1) > buf_len)
        return NULL;

    size_t line_len = strcspn(buf + *start_idx, "\n");
    size_t line_size = line_len + 1;
    char *line = arena_alloc(arena, line_size);
    memset(line, 0, line_size);

    size_t i = 0;

    while (i < line_len) {
        char c = buf[*start_idx + i];

        if (c == '\n' || c == '\0')
            break;

        line[i++] = c;
    }

    line[i] = '\0';

    *start_idx += line_size; // line_size works here because the +1 takes the
                             // role of skipping \n

    return line;
}
