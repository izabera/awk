#ifndef VAR_H
#define VAR_H
struct var {
    char *key;
    void *value;
    size_t size;
};

extern struct var *vars;

struct query {
    const char *name;
    const char *index;
    const char *value;
    void *(*search)(const void *key, void **rootp, int (*compar)(const void *, const void *));
};

struct var *var_(struct query);
#include <search.h>
#define var(...) var_((struct query) { __VA_ARGS__, .search = tsearch })
#define value(...) ((char *)(var(__VA_ARGS__)->value))
#define in(...) (var_)((struct query) { __VA_ARGS__, .search = (void *)tfind })
#endif
