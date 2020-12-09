#include <string.h>
#include <stdlib.h>
#include "var.h"

static int varcmp(const void *a, const void *b) {
    const struct var *va = a, *vb = b;
    return strcmp(va->key, vb->key);
}

struct var *vars;

struct var *var(struct query query) {
#define makeentry(...) memcpy(malloc(sizeof(struct var)), &(struct var) { __VA_ARGS__ }, sizeof(struct var))
#define freeentry(entry) do { if (entry) { free(entry->key); free(entry->value); free(entry); } } while (0)
    struct var *entry = makeentry(strdup(query.name));

    struct var **v = query.search(entry, (void **)&vars, varcmp);
    if (!v) goto end; // tfind failed

    if (*v == entry)
        entry = NULL; // give away ownership of the new entry we just inserted

    if (!query.index) { // we requested a single element
        if (query.value) {
            free((*v)->value);
            (*v)->value = strdup(query.value);
        }
        goto end;
    }

    entry = makeentry(.key = strdup(query.index), .size = -1, .value = strdup(query.value ? query.value : ""));

    v = query.search(entry, (void **)&(*v)->value, varcmp);
    if (!v) // tfind failed
        goto end;

    if (*v != entry && query.value) {
        free((*v)->value);
        (*v)->value = entry->value;
        goto end;
    }
    entry = NULL;

end:
    freeentry(entry);
    return v ? *v : NULL;
}
