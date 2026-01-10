#include <starlang/frontend.h>

#include <assert.h>
#include <math.h>
#include <stdio.h>

void frontend_prettyprint_errpos(char *line_str, size_t line_count, size_t col,
                                 size_t point_len) {
    assert(line_count > 0);

    // +4 for the 3 spaces and one '|' in the line info string
    // log10(n) + 1 is used to calculate the length of any number
    size_t line_info_len = (log10(line_count) + 1) + 4;
    size_t line_info_size = line_info_len + 1;

    char line_info[line_info_size];
    snprintf(line_info, line_info_size, " %zu | ", line_count);

    printf("%s", line_info);
    printf("%s\n", line_str);

    int point_offset = (int)line_info_len + (int)col - 1;
    printf("%*s", point_offset, " ");

    size_t print_count = 0;
    while (print_count++ < point_len) {
        printf("^");
    }

    printf("\n");
}
