#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <search.h>

#include "split.h"
#include "var.h"
#include "vector.h"

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
