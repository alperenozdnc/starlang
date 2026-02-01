
#include <starlang/arena.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define KiB(n) (n << 10)
#define MiB(n) (KiB(n) << 10)
#define GiB(n) (MiB(n) << 10)

/*
 * checks for existence of any kind of object on path `pathname`. returns a
 * boolean based on success.
 */
bool util_does_path_exist(char *pathname);

/*
 * checks if a given path `pathname` is a file. returns a boolean based on
 * success. this utility assumes `pathname` is already validated to exist on
 * disk.
 */
bool util_is_path_a_file(char *pathname);

/*
 * checks if a given path `pathname` has extension `ext`. returns a boolean
 * based on success. this utility assumes `pathname` is already validated to
 * exist on disk, and is validated to be a regular file. note that `ext` has to
 * be prefixed by a dot.
 */
bool util_validate_file_ext(char *pathname, char *ext);

/*
 * gets a file's size in terms of total bytes. this utility assumes `pathname`
 * is already validated to exist on disk, and is validated to be a regular file.
 */
ssize_t util_get_file_size(FILE *f);

/*
 * strdup's a file's content into a given arena.
 */
char *util_read_file_into_arena(arena_t *arena, FILE *f);

/*
 * reads all of the characters before a `\n` or a `\0` is seen, and returns them
 * in a string. starts from `start_idx`. automatically mutates `start_idx` to
 * the next line, `line_len` to the length of the current line read, and
 * `lines_size` to the amount of lines read.
 */
char *util_read_line(arena_t *arena, char *buf, size_t buf_len,
                     size_t *line_len, size_t *lines_size, size_t *start_idx);

/*
 * checks if a string ends with suffix string `suffix`. returns a boolean value
 * based on success.
 */
bool util_does_str_end_with_suffix(char *str, char *suffix);

// enum for deciding which kinds of characters to whitelist
typedef enum {
    CHAR_ALPHA = 1 << 0,
    CHAR_DIGIT = 1 << 1,
    CHAR_SYMBOL = 1 << 2
} char_whitelist_t;

/*
 * enforces a matching rule over every character in string `str` with regards to
 * exceptions. exceptions should be any character that would normally fail if
 * only looking at `whitelist`, but needs to be allowed. `exceptions` must be
 * `NULL` terminated. returns the offending character if any is found, returns
 * '\0' otherwise. `char_whitelist_t` values are to be used with bit masking.
 */
char util_enforce_str_rules(char *str, size_t len, char_whitelist_t whitelist,
                            char exceptions[]);

/*
 * builds an arbitrary amount of strings passed as arguments into one whole
 * string. you should use the `util_build_str` macro instead of using this
 * directly.
 */
char *_util_build_str(arena_t *arena, ...);

/*
 * counts the amount of lines in a file.
 */
size_t util_count_lines(FILE *fptr);

#define util_build_str(arena, ...) _util_build_str(arena, __VA_ARGS__, NULL)
