#pragma once

#include <ezcli.h>

// yes this does use cliprint which is an ezcli primitive but it's definitely
// better to have a unified printer
#define FRONTEND_ERR CLI_ERROR
#define FRONTEND_WARN CLI_WARN
#define FRONTEND_HINT CLI_HINT
#define FRONTEND_PRINT_PREFIX "[starlang] "

#define FRONTEND_PRINT(code, prefix, msg, ...)                                 \
    cliprint(code, prefix, msg,                                                \
             ##__VA_ARGS__) // '##' makes var args not required

#define FRONTEND_THROW_ERR_WITH_POS(line_str, line, col, len, msg, ...)        \
    do {                                                                       \
        frontend_prettyprint_errpos(line_str, line, col, len);                 \
                                                                               \
        FRONTEND_PRINT(FRONTEND_ERR, FRONTEND_PRINT_PREFIX, msg,               \
                       ##__VA_ARGS__);                                         \
                                                                               \
        exit(EXIT_FAILURE);                                                    \
    } while (0);

/*
 * interprets starlang code in given filename. returns boolean based on success.
 */
bool frontend_entrypoint(const char *full_path, const char *path,
                         const char *filename);

#define CLI_CMD_NAME "starlang"

#define CLI_DESC "an interpreted language made for educational purposes."

#define CLI_USAGE "{filename.st}"

#define CLI_FOOTER                                                             \
    "project license: "                                                        \
    "<https://www.gnu.org/licenses/gpl-3.0.en.html>.\nsource code: "           \
    "<https://github.com/alperenozdnc/starlang>\n"

#define CLI_HELP_ALIASES CLI_ALIASES("help", "--help")

/*
 * initializes a cli instance.
 */
void frontend_cli(int argc, char *argv[]);

/*
 * prints the offending line for an error, and draws carets under it that point
 * to the offending characters.
 */
void frontend_prettyprint_errpos(char *line_str, size_t line_count, size_t col,
                                 size_t point_len);
