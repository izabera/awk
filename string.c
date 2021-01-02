#include <stdlib.h>
#include <string.h>
#include "string.h"

static struct elem {
    string s;
    struct elem *next;
} *strings;
static int size = 1;

// 0x1e == 00011110
//            ^^^^  bytes left
//                ^ on heap

static const string empty = { .buf[sizeof empty.buf - 1] = 0x1e };

// stralloc always returns a 
string *stralloc() {
    if (!strings) {
        size *= 2;
        strings = malloc(size * sizeof(struct elem));
        for (int i = 0; i < size - 1; i++) {
            strings[i].next = strings + i + 1;
            strings[i].s = empty;
        }
        strings[size - 1].next = NULL;
        strings[size - 1].s = empty;
    }
    struct elem *e = strings;
    strings = strings->next;
    return (string *)e;
}

void strfree(string *s) {
    struct elem *e = (struct elem *)s;
    e->next = strings;
    strings = e;
    if (onheap(s))
        free(str(s));
    *s = empty;
}

string *strnew(const char *ptr, size_t size /* + 1 for the \0 */) {
    string *s = stralloc();
    if (size <= sizeof s->buf - 1) {
        memcpy(s->buf, ptr, size);
        s->buf[size] = 0;
    }
    else {
        s->heap = 1;
        s->size = size;
        s->ptr = malloc(size + 1);
        memcpy(s->ptr, ptr, size);
        s->ptr[size] = 0;
    }
    return s;
}
