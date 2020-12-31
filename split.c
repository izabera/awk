#include <stdlib.h>
#include "vector.h"
#include "split.h"

void regexsplit(regex_t *regex, const char *str, vregmatch *delims) {
    for (size_t i = 0; ; i++) {
        if (delims->used == delims->alloc)
        {
            delims->alloc = delims->used * 2 + 1;
            delims->data = realloc(delims->data, delims->alloc * sizeof delims->data[0]);
        }
        if (regexec(regex, str, 1, delims->data + i, 0))
            break;
        delims->used++;
        str += delims->data[i].rm_eo;
    }
}
