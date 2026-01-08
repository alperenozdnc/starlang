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

/*
 * interprets starlang code in given filename. returns boolean based on success.
 */
bool frontend_entrypoint(const char *filename);

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
