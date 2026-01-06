#include <starlang/frontend.h>

#include <stdio.h>

bool frontend_entrypoint(char *filename) {
    printf("assume this runs '%s'.\n", filename);

    return true;
}
