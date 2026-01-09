#include <starlang/def.h>
#include <starlang/frontend.h>
#include <starlang/utils.h>

#include <ezcli.h> // my project too, see in github

static bool did_run_main_opt = false;

ret_e main_opt_body(CLI_IGNORE_CTX, char *pathname) {
    if (did_run_main_opt) {
        FRONTEND_PRINT(FRONTEND_ERR, FRONTEND_PRINT_PREFIX,
                       "interpreter can only execute a single file at a time.");

        return RET_FAIL;
    }

    did_run_main_opt = true;

    if (!util_does_path_exist(pathname)) {
        FRONTEND_PRINT(FRONTEND_ERR, FRONTEND_PRINT_PREFIX,
                       "path '%s' doesn't exist on disk.", pathname);

        return RET_FAIL;
    }

    if (!util_is_path_a_file(pathname)) {
        FRONTEND_PRINT(FRONTEND_ERR, FRONTEND_PRINT_PREFIX,
                       "path '%s' isn't a file.", pathname);

        return RET_FAIL;
    }

    if (!util_validate_file_ext(pathname, STARLANG_FILE_EXT)) {
        FRONTEND_PRINT(FRONTEND_WARN, FRONTEND_PRINT_PREFIX,
                       "starlang files should have '%s' as their "
                       "extension for clarity.",
                       STARLANG_FILE_EXT);
    }

    if (frontend_entrypoint(pathname))
        return RET_NORMAL;

    FRONTEND_PRINT(FRONTEND_ERR, FRONTEND_PRINT_PREFIX,
                   "failed to run '%s'. exiting.", pathname);

    return RET_FAIL;
}

opt_s main_opt = {
    .aliases = CLI_ALIASES(CLI_NONOPT),
    .body = main_opt_body,
};

void frontend_cli(int argc, char *argv[]) {
    CLI_ALLOW_NONOPT = true;
    cli_s cli = {0};
    opt_s *opts[] = {&main_opt, NULL};

    initcli(&cli, CLI_CMD_NAME, CLI_DESC, CLI_USAGE, CLI_FOOTER, opts,
            CLI_HELP_ALIASES);

    runcli(&cli, argc, argv);

    freecli(&cli);
}
