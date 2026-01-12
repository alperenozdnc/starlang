#include <starlang/arena.h>
#include <starlang/utils.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

char *_util_build_str(arena_t *arena, ...) {
    size_t str_size =
        256; // reasonable first start size, growth is geometric from then on
    size_t in_use = 0;

    char *heap_str = malloc(str_size);
    memset(heap_str, 0, str_size);

    va_list strings;
    va_start(strings, arena);

    while (true) {
        char *str = va_arg(strings, char *);

        if (str == NULL) { // we accept NULL as sentinel
            break;
        }

        in_use += strlen(str) + 1;

        if (in_use > str_size) {
            str_size = MAX(in_use, str_size * 2);
            heap_str = realloc(heap_str, str_size);
        }

        strcat(heap_str, str);
    }

    va_end(strings);

    size_t final_size = strlen(heap_str) + 1;

    char *arena_str = arena_alloc(arena, final_size);
    memset(arena_str, 0, final_size);

    strcpy(arena_str, heap_str);
    free(heap_str);

    return arena_str;
}
