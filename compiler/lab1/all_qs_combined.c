#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1000

/* ============================================================================
   Shared Helpers
   ============================================================================ */

/* Read entire file into buffer */
static int read_file_to_buffer(const char *filename, char **out, long *out_size)
{
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = (char *)malloc(sz + 1);
    if (!buf) { fclose(f); return 0; }
    fread(buf, 1, sz, f);
    buf[sz] = '\0';
    fclose(f);
    *out = buf;
    if (out_size) *out_size = sz;
    return 1;
}


/* Check if line is blank (only whitespace) */
static int is_blank_line(const char *line)
{
    for (int i = 0; line[i] != '\0'; i++) {
        if (!isspace((unsigned char)line[i])) return 0;
    }
    return 1;
}

/* Check if line starts with comment token */
static int is_comment_line(const char *line)
{
    int i = 0;
    while (isspace((unsigned char)line[i])) i++;
    if (line[i] == '/' && line[i + 1] == '/') return 1;
    if (line[i] == '/' && line[i + 1] == '*') return 1;
    return 0;
}


/* Remove comments but preserve string literals */
static char *remove_comments_and_preserve_strings(const char *content, long size)
{
    char *out = malloc(size + 1);
    if (!out) return NULL;
    int i = 0, j = 0, in_single = 0, in_multi = 0, in_string = 0;
    while (i < size) {
        if (!in_single && !in_multi && content[i] == '"' && (i == 0 || content[i - 1] != '\\')) {
            in_string = !in_string;
            out[j++] = content[i++];
            continue;
        }
        if (in_string) { out[j++] = content[i++]; continue; }
        if (!in_multi && content[i] == '/' && content[i + 1] == '/') {
            in_single = 1;
            i += 2;
            continue;
        }
        if (in_single) {
            if (content[i] == '\n') { in_single = 0; out[j++] = '\n'; }
            i++;
            continue;
        }
        if (!in_single && content[i] == '/' && content[i + 1] == '*') {
            in_multi = 1;
            i += 2;
            continue;
        }
        if (in_multi) {
            if (content[i] == '*' && content[i + 1] == '/') { in_multi = 0; i += 2; }
            else i++;
            continue;
        }
        out[j++] = content[i++];
    }
    out[j] = '\0';
    return out;
}

/* ============================================================================
   Q1: Count lines, words, characters
   ============================================================================ */

void q1_count_lines_words_chars(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) { printf("Error: Could not open file %s\n", filename); return; }
    int lines = 0, words = 0, characters = 0, in_word = 0, ch;
    while ((ch = fgetc(file)) != EOF) {
        characters++;
        if (ch == '\n') lines++;
        if (isspace(ch)) in_word = 0;
        else if (!in_word) { in_word = 1; words++; }
    }
    fclose(file);
    printf("\n=== Q1: Lines, Words, and Characters ===\nFile: %s\nLines: %d\nWords: %d\nCharacters: %d\n", filename, lines, words, characters);
}

/* ============================================================================
   Q2: Count blank, comment, and code lines
   ============================================================================ */

void q2_count_blank_comment_code(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) { printf("Error: Could not open file %s\n", filename); return; }
    char line[MAX_LINE];
    int blank = 0, comment = 0, code = 0, in_multiline = 0;
    while (fgets(line, MAX_LINE, file) != NULL) {
        if (in_multiline) {
            comment++;
            if (strstr(line, "*/")) in_multiline = 0;
            continue;
        }
        if (strstr(line, "/*") != NULL) {
            comment++;
            in_multiline = 1;
            if (strstr(line, "*/") != NULL) in_multiline = 0;
            continue;
        }
        if (is_blank_line(line)) blank++;
        else if (is_comment_line(line)) comment++;
        else code++;
    }
    fclose(file);
    printf("\n=== Q2: Blank, Comment, and Code Lines ===\nFile: %s\nBlank Lines: %d\nComment Lines: %d\nCode Lines: %d\nTotal Lines: %d\n",
           filename, blank, comment, code, blank + comment + code);
}

/* ============================================================================
   Q3: Count function definitions (heuristic)
   ============================================================================ */

void q3_count_functions(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) { printf("Error: Could not open file %s\n", filename); return; }
    char line[MAX_LINE], prev[MAX_LINE] = "";
    int count = 0, in_multi = 0;
    while (fgets(line, MAX_LINE, file) != NULL) {
        if (strstr(line, "/*")) in_multi = 1;
        if (in_multi) { if (strstr(line, "*/")) in_multi = 0; continue; }
        int i = 0;
        while (isspace((unsigned char)line[i])) i++;
        if (line[i] == '/' && line[i + 1] == '/') continue;
        if (strchr(line, '{')) {
            if (strchr(prev, ')') || strchr(line, ')')) {
                char temp[MAX_LINE * 2];
                strcpy(temp, prev);
                strcat(temp, line);
                char lower[MAX_LINE * 2];
                int k = 0;
                for (int j = 0; temp[j]; j++) lower[k++] = tolower((unsigned char)temp[j]);
                lower[k] = '\0';
                if (!strstr(lower, "if") && !strstr(lower, "for") && !strstr(lower, "while") &&
                    !strstr(lower, "switch") && !strstr(lower, "struct") && !strstr(lower, "enum") &&
                    !strstr(lower, "union")) count++;
            }
        }
        strcpy(prev, line);
    }
    fclose(file);
    printf("\n=== Q3: Function Count ===\nFile: %s\nNumber of Functions: %d\n", filename, count);
}

/* ============================================================================
   Q4: Count header includes
   ============================================================================ */

void q4_count_header_files(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) { printf("Error: Could not open file %s\n", filename); return; }
    char line[MAX_LINE], headers[100][200];
    int hdr_count = 0, total_includes = 0, in_multi = 0;
    while (fgets(line, MAX_LINE, file) != NULL) {
        if (strstr(line, "/*")) in_multi = 1;
        if (in_multi) { if (strstr(line, "*/")) in_multi = 0; continue; }
        int i = 0;
        while (isspace((unsigned char)line[i])) i++;
        if (line[i] == '/' && line[i + 1] == '/') continue;
        if (line[i] == '#') {
            i++;
            while (isspace((unsigned char)line[i])) i++;
            if (strncmp(&line[i], "include", 7) == 0) {
                i += 7;
                while (isspace((unsigned char)line[i])) i++;
                if (line[i] == '<' || line[i] == '"') {
                    char closing = (line[i] == '<') ? '>' : '"';
                    i++;
                    char name[200];
                    int j = 0;
                    while (line[i] && line[i] != closing && line[i] != '\n') name[j++] = line[i++];
                    name[j] = '\0';
                    if (j > 0) {
                        total_includes++;
                        int found = 0;
                        for (int k = 0; k < hdr_count; k++) {
                            if (strcmp(headers[k], name) == 0) { found = 1; break; }
                        }
                        if (!found) strcpy(headers[hdr_count++], name);
                    }
                }
            }
        }
    }
    fclose(file);
    printf("\n=== Q4: Header Files ===\nFile: %s\nTotal #include statements: %d\nUnique header files: %d\n\n", filename, total_includes, hdr_count);
    if (hdr_count > 0) {
        printf("Header files included:\n");
        for (int i = 0; i < hdr_count; i++) printf("  %s\n", headers[i]);
    }
}

/* ============================================================================
   Q5: Count loops and conditionals
   ============================================================================ */

static int count_keyword(const char *content, const char *keyword)
{
    int count = 0;
    const char *ptr = content;
    int klen = (int)strlen(keyword);
    while ((ptr = strstr(ptr, keyword)) != NULL) {
        int whole = 1;
        if (ptr != content) {
            char prev = *(ptr - 1);
            if (isalnum((unsigned char)prev) || prev == '_') whole = 0;
        }
        if (whole) {
            char next = *(ptr + klen);
            if (isalnum((unsigned char)next) || next == '_') whole = 0;
        }
        if (whole) count++;
        ptr++;
    }
    return count;
}

void q5_count_loops_conditionals(const char *filename)
{
    char *content = NULL;
    long sz = 0;
    if (!read_file_to_buffer(filename, &content, &sz)) { printf("Error: Could not open file %s\n", filename); return; }
    char *clean = remove_comments_and_preserve_strings(content, sz);
    if (!clean) { free(content); printf("Memory error\n"); return; }
    int for_loops = count_keyword(clean, "for");
    int while_loops = count_keyword(clean, "while");
    int do_loops = count_keyword(clean, "do");
    int ifs = count_keyword(clean, "if");
    int sw = count_keyword(clean, "switch");
    printf("\n=== Q5: Loops and Conditionals ===\nFile: %s\n\nLoops:\n  for: %d\n  while: %d\n  do-while: %d\n  Total loops: %d\n\nConditionals:\n  if: %d\n  switch: %d\n  Total conditionals: %d\n",
           filename, for_loops, while_loops, do_loops, for_loops + while_loops + do_loops, ifs, sw, ifs + sw);
    free(content);
    free(clean);
}

/* ============================================================================
   Q6: Count variables by data type (heuristic)
   ============================================================================ */

typedef struct {
    int int_count, float_count, double_count, char_count, long_count;
    int short_count, unsigned_count, signed_count, void_count;
} TypeCount;

static int is_keyword_simple(const char *word)
{
    const char *keywords[] = {
        "if","else","while","for","do","switch","case","break","continue","return",
        "sizeof","typedef","struct","union","enum","const","static","extern","auto",
        "register","volatile","goto","default"
    };
    int n = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < n; i++) if (strcmp(word, keywords[i]) == 0) return 1;
    return 0;
}

static void count_variables_by_type_from_content(const char *content, TypeCount *counts)
{
    const char *types[] = {"int","float","double","char","long","short","unsigned","signed","void"};
    int num_types = 9;
    const char *ptr = content;
    char word[256];
    while (*ptr) {
        while (isspace((unsigned char)*ptr)) ptr++;
        if (!*ptr) break;
        int i = 0;
        if (isalpha((unsigned char)*ptr) || *ptr == '_') {
            while (isalnum((unsigned char)*ptr) || *ptr == '_') word[i++] = *ptr++;
            word[i] = '\0';
            int tindex = -1;
            for (int j = 0; j < num_types; j++) if (strcmp(word, types[j]) == 0) { tindex = j; break; }
            if (tindex != -1) {
                while (isspace((unsigned char)*ptr) || *ptr == '*') ptr++;
                while (*ptr && *ptr != ';' && *ptr != ')') {
                    while (isspace((unsigned char)*ptr) || *ptr == '*') ptr++;
                    if (isalpha((unsigned char)*ptr) || *ptr == '_') {
                        int k = 0;
                        char name[256];
                        while (isalnum((unsigned char)*ptr) || *ptr == '_') name[k++] = *ptr++;
                        name[k] = '\0';
                        while (isspace((unsigned char)*ptr)) ptr++;
                        if (*ptr == '(') {
                            int pc = 1;
                            ptr++;
                            while (*ptr && pc > 0) { if (*ptr == '(') pc++; if (*ptr == ')') pc--; ptr++; }
                        } else if (*ptr == '[' || *ptr == ',' || *ptr == ';' || *ptr == '=' || *ptr == ')') {
                            if (!is_keyword_simple(name)) {
                                switch (tindex) {
                                    case 0: counts->int_count++; break;
                                    case 1: counts->float_count++; break;
                                    case 2: counts->double_count++; break;
                                    case 3: counts->char_count++; break;
                                    case 4: counts->long_count++; break;
                                    case 5: counts->short_count++; break;
                                    case 6: counts->unsigned_count++; break;
                                    case 7: counts->signed_count++; break;
                                    case 8: counts->void_count++; break;
                                }
                            }
                        }
                    } else ptr++;
                    while (*ptr && *ptr != ',' && *ptr != ';' && *ptr != ')') ptr++;
                    if (*ptr == ',') ptr++; else break;
                }
            }
        } else ptr++;
    }
}

void q6_count_variables_by_type(const char *filename)
{
    char *content = NULL;
    long sz = 0;
    if (!read_file_to_buffer(filename, &content, &sz)) { printf("Error: Could not open file %s\n", filename); return; }
    char *clean = remove_comments_and_preserve_strings(content, sz);
    if (!clean) { free(content); printf("Memory error\n"); return; }
    TypeCount counts = {0};
    count_variables_by_type_from_content(clean, &counts);
    printf("\n=== Q6: Variable Count by Data Type ===\nFile: %s\n\nVariable counts:\n  int: %d\n  float: %d\n  double: %d\n  char: %d\n  long: %d\n  short: %d\n  unsigned: %d\n  signed: %d\n  void: %d\n\nTotal variables: %d\n",
           filename, counts.int_count, counts.float_count, counts.double_count, counts.char_count, counts.long_count, counts.short_count, counts.unsigned_count, counts.signed_count, counts.void_count,
           counts.int_count + counts.float_count + counts.double_count + counts.char_count + counts.long_count + counts.short_count + counts.unsigned_count + counts.signed_count);
    free(content);
    free(clean);
}



/* ============================================================================
   Q7: Remove extra spaces from source code
   ============================================================================ */

void q7_remove_extra_spaces(const char *input_filename, const char *output_filename)
{
    FILE *input = fopen(input_filename, "r");
    if (!input) { printf("Error opening %s\n", input_filename); return; }
    FILE *output = fopen(output_filename, "w");
    if (!output) { printf("Error creating %s\n", output_filename); fclose(input); return; }
    int ch, prev_ch = '\0', in_string = 0, in_char = 0, in_single_comment = 0, in_multi_comment = 0;
    int space_written = 0, at_line_start = 1;
    while ((ch = fgetc(input)) != EOF) {
        if (ch == '"' && prev_ch != '\\' && !in_char && !in_single_comment && !in_multi_comment) { in_string = !in_string; fputc(ch, output); prev_ch = ch; space_written = 0; at_line_start = 0; continue; }
        if (ch == '\'' && prev_ch != '\\' && !in_string && !in_single_comment && !in_multi_comment) { in_char = !in_char; fputc(ch, output); prev_ch = ch; space_written = 0; at_line_start = 0; continue; }
        if (in_string || in_char) { fputc(ch, output); prev_ch = ch; continue; }
        if (ch == '/' && prev_ch == '/' && !in_multi_comment) { in_single_comment = 1; fputc(ch, output); prev_ch = ch; space_written = 0; continue; }
        if (in_single_comment) { fputc(ch, output); if (ch == '\n') { in_single_comment = 0; at_line_start = 1; space_written = 0; } prev_ch = ch; continue; }
        if (ch == '*' && prev_ch == '/' && !in_single_comment) { in_multi_comment = 1; fputc(ch, output); prev_ch = ch; space_written = 0; continue; }
        if (in_multi_comment) { fputc(ch, output); if (ch == '/' && prev_ch == '*') in_multi_comment = 0; if (ch == '\n') { at_line_start = 1; space_written = 0; } else if (!isspace(ch)) at_line_start = 0; prev_ch = ch; continue; }
        if (ch == '\n') { fputc(ch, output); at_line_start = 1; space_written = 0; prev_ch = ch; continue; }
        if (isspace(ch)) { if (at_line_start) { prev_ch = ch; continue; } if (!space_written) { fputc(' ', output); space_written = 1; } prev_ch = ch; continue; }
        fputc(ch, output); space_written = 0; at_line_start = 0; prev_ch = ch;
    }
    fclose(input);
    fclose(output);
    printf("\n=== Q7: Space Removal ===\nInput file: %s\nOutput file: %s\n", input_filename, output_filename);
}

int main(void)
{
    char filename[512];
    printf("Enter the C source filename to analyze: ");
    if (scanf("%511s", filename) != 1) return 0;
    q1_count_lines_words_chars(filename);
    q2_count_blank_comment_code(filename);
    q3_count_functions(filename);
    q4_count_header_files(filename);
    q5_count_loops_conditionals(filename);
    q6_count_variables_by_type(filename);
    char outname[1024];
    const char *dot = strrchr(filename, '.');
    if (dot) {
        size_t base_len = (size_t)(dot - filename);
        if (base_len > 900) base_len = 900;
        memcpy(outname, filename, base_len);
        outname[base_len] = '\0';
        strcat(outname, "_nospaces.c");
    } else {
        strncpy(outname, filename, sizeof(outname) - 1);
        outname[sizeof(outname)-1] = '\0';
        strcat(outname, "_nospaces.c");
    }
    q7_remove_extra_spaces(filename, outname);
    return 0;
}
