#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

#include "split.h"
#include "var.h"
#include "vector.h"

typedef struct { const char *start, *end; } token;
vector(token) tokens;

void tokenize(const char *s) {
    for (const char *end = s; *s; s = end) {
        if (isspace(*s)) {
            end++;
            continue;
        }

        if (isalpha(*s))
            for ( ; isalpha(*end) || isdigit(*end); end++);

        else if (isdigit(*s)) {
            for ( ; isdigit(*end); end++);
            if (*end == '.')
                for (end++; isdigit(*end); end++);
            if (*end == 'E' || *end == 'e') {
                end++;
                if (*end == '-' || *end == '+')
                    end++;
                for ( ; isdigit(*end); end++);
            }
        }

        else if (*s == '"') {
            for (end++; *end != '"'; end++)
                if (*end == '\\')
                    end++;
            end++;
        }

        else
            end++;

        token t = { s, end };
        vecpush(tokens, t);
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        tokenize(argv[1]);
        printf("%zu tokens:\n", tokens.used);
        for (size_t i = 0; i < tokens.used; i++)
            printf("%zu = {%.*s}\n", i, (int) (tokens.data[i].end - tokens.data[i].start), tokens.data[i].start);
    }

    /*
    char *line = NULL;
    size_t n = 0;
    ssize_t readsize;

    FILE *input = stdin;
#define RS (value("RS")[0])
    while (readsize = getdelim(&line, &n, RS, input)) {
    }
    */
}
