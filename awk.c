#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <search.h>

struct var {
    char *key;
    void *value;
    size_t size;
};

struct var *vars;

static int varcmp(const void *a, const void *b) {
    const struct var *va = a, *vb = b;
    return strcmp(va->key, vb->key);
}

struct query {
    const char *name;
    const char *index;
    const char *value;
    void *(*search)(const void *key, void **rootp, int (*compar)(const void *, const void *));
};

struct var *var(struct query query) {
#define makeentry(...) memcpy(malloc(sizeof(struct var)), &(struct var) { __VA_ARGS__ }, sizeof(struct var))
#define freeentry(entry) do { if (entry) { free(entry->key); free(entry->value); free(entry); } } while (0)
    struct var *entry = makeentry(strdup(query.name));

    struct var **v = query.search(entry, (void **)&vars, varcmp);
    if (!v) goto end; // tfind failed

    if (*v == entry)
        entry = NULL; // give away ownership of the new entry we just inserted

    if (!query.index) { // we requested a single element
        if (query.value)
            (*v)->value = strdup(query.value);
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
    return v ? *v : v;
}





int main(/* int argc, char *argv[] */) {

#define var(...) var((struct query) { __VA_ARGS__, .search = tsearch })
#define value(...) ((char *)(var(__VA_ARGS__)->value))
#define in(...) (var)((struct query) { __VA_ARGS__, .search = (void *)tfind })

    puts("foo = \"meow\"");
    var("foo", .value = "meow");

    puts("bar = \"mooo\"");
    var("bar", .value = "mooo");

    puts("baz[42] = \"hello\"");
    var("baz", "42", "hello");

    puts("baz[\"asdf\"] = \"world\"");
    var("baz", "asdf", "world");

    puts("");


    printf("%-30s -> %s\n", "print foo", value("foo"));
    printf("%-30s -> %s\n", "print foo = \"awk\"", value("foo", .value = "awk"));

    printf("%-30s -> %s\n", "print bar", value("bar"));
    printf("%-30s -> %s\n", "print baz[42]", value("baz", "42"));
    printf("%-30s -> %s\n", "print baz[\"asdf\"]", value("baz", "asdf"));

    printf("%-30s -> %s\n", "42 in baz", in("baz", "42") ? "true" : "false");
    printf("%-30s -> %s\n", "\"fffff\" in baz", in("baz", "fffff") ? "true" : "false");
    /*
    char *line = NULL;
    size_t n = 0;
    ssize_t readsize;

    FILE *input = stdin;
#define RS (var("RS")[0])
    while (readsize = getdelim(&line, &n, RS, input)) {
    }
    */
}
