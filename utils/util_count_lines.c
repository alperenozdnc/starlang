#include <starlang/utils.h>

size_t util_count_lines(FILE *fptr) {
    size_t line_count = 0;

    char c;

    for (c = getc(fptr); c != EOF; c = getc(fptr)) {
        if (c == '\n')
            line_count++;
    }

    return line_count;
}
