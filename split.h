#ifndef SPLIT_H
#define SPLIT_H
#include <regex.h>
#include "vector.h"
void regexsplit(regex_t *regex, const char *str, struct vector *delims);
#endif
