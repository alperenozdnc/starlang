#include <starlang/replacer.h>

#include <stdbool.h>
#include <string.h>

bool replacer_is_dep_uniq(nmspc_link_t *head, nmspc_module_t *dep) {
    nmspc_link_t *link = head;

    while (link) {
        if ((strcmp(link->self->namespace, dep->namespace) != 0) ||
            (strcmp(link->self->module, dep->module) != 0)) {
            bool curr_branch_ok = replacer_is_dep_uniq(link->children, dep);

            if (curr_branch_ok) {
                link = link->next;

                continue;
            }
        }

        return false;
    }

    return true;
}
