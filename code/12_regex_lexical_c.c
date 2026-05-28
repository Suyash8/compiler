#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    const char *name;
    regex_t regex;
} TokenRule;

static void fail_regex(int code, regex_t *regex, const char *pattern)
{
    char buffer[256];
    regerror(code, regex, buffer, sizeof(buffer));
    fprintf(stderr, "Regex error for pattern %s: %s\n", pattern, buffer);
    exit(1);
}

static char *read_file(const char *path)
{
    FILE *file = fopen(path, "r");
    if (!file)
    {
        return NULL;
    }
    if (fseek(file, 0, SEEK_END) != 0)
    {
        fclose(file);
        return NULL;
    }
    long size = ftell(file);
    if (size < 0)
    {
        fclose(file);
        return NULL;
    }
    rewind(file);
    char *buffer = malloc((size_t)size + 1);
    if (!buffer)
    {
        fclose(file);
        return NULL;
    }
    size_t read_size = fread(buffer, 1, (size_t)size, file);
    buffer[read_size] = '\0';
    fclose(file);
    return buffer;
}

static void add_rule(TokenRule *rules, int index, const char *name, const char *pattern)
{
    rules[index].name = name;
    int code = regcomp(&rules[index].regex, pattern, REG_EXTENDED);
    if (code != 0)
    {
        fail_regex(code, &rules[index].regex, pattern);
    }
}

static void print_token(const char *type, const char *text, int length)
{
    printf("%-12s %.*s\n", type, length, text);
}

int main(int argc, char **argv)
{
    const char *input_file = argc > 1 ? argv[1] : "code/sample_inputs/01_sample_input.c";
    char *source = read_file(input_file);
    if (!source)
    {
        printf("Error: unable to open file: %s\n", input_file);
        return 1;
    }

    TokenRule rules[9];
    add_rule(rules, 0, "COMMENT", "^//.*");
    add_rule(rules, 1, "INCLUDE", "^#[[:space:]]*include[[:space:]]*<[^>]+>");
    add_rule(rules, 2, "STRING", "^\"([^\"\\\\]|\\\\.)*\"");
    add_rule(rules, 3, "NUMBER", "^[0-9]+(\\.[0-9]+)?");
    add_rule(rules, 4, "IDENTIFIER", "^[A-Za-z_][A-Za-z0-9_]*");
    add_rule(rules, 5, "OPERATOR", "^(==|!=|<=|>=|\\+\\+|--|[-+*/%=<>])");
    add_rule(rules, 6, "DELIMITER", "^[(){};,]");
    add_rule(rules, 7, "WHITESPACE", "^[[:space:]]+");
    add_rule(rules, 8, "BLOCK_COMMENT", "^/\\*([^*]|\\*+[^*/])*\\*+/");

    const char *keywords[] = {
        "int", "float", "double", "char", "void", "return", "if", "else",
        "for", "while", "do", "switch", "case", "break", "continue", "main"};
    int keyword_count = (int)(sizeof(keywords) / sizeof(keywords[0]));

    printf("Input file: %s\n", input_file);
    printf("Tokens:\n");

    char *cursor = source;
    while (*cursor != '\0')
    {
        int matched = 0;
        for (int i = 0; i < 9; i++)
        {
            regmatch_t match;
            if (regexec(&rules[i].regex, cursor, 1, &match, 0) == 0 && match.rm_so == 0)
            {
                int length = (int)match.rm_eo;
                if (strcmp(rules[i].name, "WHITESPACE") != 0)
                {
                    if (strcmp(rules[i].name, "IDENTIFIER") == 0)
                    {
                        int is_keyword = 0;
                        for (int k = 0; k < keyword_count; k++)
                        {
                            if ((int)strlen(keywords[k]) == length && strncmp(cursor, keywords[k], (size_t)length) == 0)
                            {
                                print_token("KEYWORD", cursor, length);
                                is_keyword = 1;
                                break;
                            }
                        }
                        if (!is_keyword)
                        {
                            print_token("IDENTIFIER", cursor, length);
                        }
                    }
                    else if (strcmp(rules[i].name, "COMMENT") == 0 || strcmp(rules[i].name, "BLOCK_COMMENT") == 0)
                    {
                        print_token("COMMENT", cursor, length);
                    }
                    else if (strcmp(rules[i].name, "INCLUDE") == 0)
                    {
                        print_token("PREPROCESSOR", cursor, length);
                    }
                    else
                    {
                        print_token(rules[i].name, cursor, length);
                    }
                }
                cursor += length;
                matched = 1;
                break;
            }
        }
        if (!matched)
        {
            if (*cursor == '\n')
            {
                cursor++;
                continue;
            }
            print_token("UNKNOWN", cursor, 1);
            cursor++;
        }
    }

    for (int i = 0; i < 9; i++)
    {
        regfree(&rules[i].regex);
    }
    free(source);
    return 0;
}