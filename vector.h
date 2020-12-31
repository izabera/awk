#ifndef VECTOR_H
#define VECTOR_H
#define vector(type) struct { size_t alloc, used; type *data; }
#endif

#include <stdlib.h>
#include <string.h>
#define vecpush(vec, ...) \
    do { \
        typeof(*vec.data) tmp[] = { __VA_ARGS__ }; \
        size_t len = sizeof tmp/sizeof *tmp; \
        if (vec.used + len > vec.alloc) \
            vec.data = realloc(vec.data, (vec.alloc = (vec.used + len) * 2) * sizeof *vec.data); \
        memcpy(vec.data + vec.used, tmp, len * sizeof *vec.data); \
        vec.used += len; \
    } while (0)
