#include <starlang/utils.h>

#include <ctype.h>
#include <stdbool.h>

char util_enforce_str_rules(const char *str, size_t len,
                            char_whitelist_t whitelist, char exceptions[]) {
    for (size_t i = 0; i < len; i++) {
        char c = str[i];

        size_t j = 0;

        while (exceptions[j])
            if (c == exceptions[j++]) {
                goto cont_outer_loop; // avoids extra bool flip & check later
            }

        unsigned char uc = (unsigned char)c;

        if ((whitelist & CHAR_ALPHA) && isalpha(uc))
            continue;

        if ((whitelist & CHAR_DIGIT) && isdigit(uc))
            continue;

        if ((whitelist & CHAR_SYMBOL) && !isalnum(uc))
            continue;

        return c;

    cont_outer_loop:
        continue;
    }

    return '\0';
}
