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

struct var *var(struct query);
#endif
