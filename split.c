#include <stdlib.h>
#include "vector.h"
#include "split.h"

void regexsplit(regex_t *regex, const char *str, struct vector *delims) {
    for (size_t i = 0; ; i++) {
        if (delims->used + sizeof(regmatch_t) > delims->alloc)
        {
            delims->alloc = delims->used * 2 + sizeof(regmatch_t);
            delims->data = realloc(delims->data, delims->alloc);
        }
        if (regexec(regex, str, 1, (regmatch_t *)delims->data + i, 0))
            break;
        delims->used += sizeof(regmatch_t);
        str += ((regmatch_t *)delims->data + i)->rm_eo;
    }
}
