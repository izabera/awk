#ifndef SPLIT_H
#define SPLIT_H
#include <regex.h>
#include "vector.h"

typedef vector(regmatch_t) vregmatch;
void regexsplit(regex_t *regex, const char *str, vregmatch *delims);
#endif
