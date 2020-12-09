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



struct vector {
    void *data;
    size_t alloc;
    size_t used;
};

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


int main(/* int argc, char *argv[] */) {

#define var(...) var((struct query) { __VA_ARGS__, .search = tsearch })
#define value(...) ((char *)(var(__VA_ARGS__)->value))
#define in(...) (var)((struct query) { __VA_ARGS__, .search = (void *)tfind })

    puts("BEGIN {");

    puts("foo = \"meow\"");
    var("foo", .value = "meow");

    puts("bar = \"mooo\"");
    var("bar", .value = "mooo");

    puts("baz[42] = \"hello\"");
    var("baz", "42", "hello");

    puts("baz[\"asdf\"] = \"world\"");
    var("baz", "asdf", "world");


#define printawk(code, result) printf("%-50s # %s\n", code, result)
    printawk("print foo", value("foo"));
    printawk("print foo = \"awk\"", value("foo", .value = "awk"));

    printawk("print bar", value("bar"));
    printawk("print baz[42]", value("baz", "42"));
    printawk("print baz[\"asdf\"]", value("baz", "asdf"));

    printawk("print 42 in baz", in("baz", "42") ? "1" : "0");
    printawk("print \"fffff\" in baz", in("baz", "fffff") ? "1" : "0");


    char *string = "12345:67-89meow000";
    char *delims = "[^0-9]+";

    printf("split(\"%s\", array, /%s/)\n", string, delims);

    regex_t regex;
    regcomp(&regex, delims, REG_EXTENDED);
    struct vector v = { };
    regexsplit(&regex, string, &v);
    regmatch_t *matches = v.data;

    int i;
    for (i = 0; i < v.used/sizeof *matches; i++) {
        char idx[20];
        sprintf(idx, "%d", i + 1);

        char value[matches[i].rm_so + 1];
        sprintf(value, "%.*s", matches[i].rm_so, string);

        var("array", idx, value);
        string += matches[i].rm_eo;
    }
    char idx[20];
    sprintf(idx, "%d", i + 1);
    var("array", idx, string);

    printawk("print array[1]", value("array", "1"));
    printawk("print array[2]", value("array", "2"));
    printawk("print array[3]", value("array", "3"));
    printawk("print array[4]", value("array", "4"));


    puts("}");

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
