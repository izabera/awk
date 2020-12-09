#ifndef VECTOR_H
#define VECTOR_H
struct vector {
    void *data;
    size_t alloc;
    size_t used;
};
#endif
