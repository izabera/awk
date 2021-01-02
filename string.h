#ifndef STRING_H
#define STRING_H

#include <inttypes.h>

typedef struct {
    union {
        char buf[16];
        struct {
            char *ptr;
            uint32_t size;
        };
        struct {
            unsigned char ignoremepls[15];
            unsigned char heap:1,
                          ssosize:4,
                          ignoremetoo:3;
        };
    };
} string;

#define onheap(s) (s->heap)
#define str(s) (onheap(s) ? s->ptr : s->buf)
#define size(s) (onheap(s) ? s->len : sizeof s->buf - s->ssosize)

string *stralloc();
void strfree(string *s);
string *strnew(const char *ptr, size_t size);

#endif
