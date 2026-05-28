#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: 12_regex_lexical_c.c <file>\n");
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f)
    {
        perror("open");
        return 1;
    }
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *s = malloc(sz + 1);
    fread(s, 1, sz, f);
    s[sz] = 0;
    fclose(f);
    regex_t re;
    regcomp(&re, "[A-Za-z_][A-Za-z0-9_]*", REG_EXTENDED);
    regmatch_t m;
    const char *p = s;
    printf("Input file: %s\n", argv[1]);
    printf("Identifiers:\n");
    while (regexec(&re, p, 1, &m, 0) == 0)
    {
        int start = m.rm_so;
        int end = m.rm_eo;
        printf("ID:%.*s\n", end - start, p + start);
        p += end;
    }
    regfree(&re);
    free(s);
    return 0;
}
