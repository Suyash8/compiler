// Q1: Count lines, words, characters
#include <stdio.h>
#include <ctype.h>

void q1(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return;
    int lines = 0, words = 0, chars = 0, in_word = 0, ch;
    while ((ch = fgetc(f)) != EOF) {
        chars++;
        if (ch == '\n') lines++;
        if (isspace(ch)) in_word = 0;
        else if (!in_word) { in_word = 1; words++; }
    }
    fclose(f);
    printf("Lines: %d, Words: %d, Chars: %d\n", lines, words, chars);
}

// Q2: Count blank, comment, code lines
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_blank(const char *line) {
    for (; *line; line++) if (!isspace(*line)) return 0;
    return 1;
}

int is_comment_start(const char *line) {
    while (isspace(*line)) line++;
    return line[0] == '/' && (line[1] == '/' || line[1] == '*');
}

void q2(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return;
    char line[1000];
    int blank = 0, comment = 0, code = 0, in_multi = 0;
    while (fgets(line, sizeof(line), f)) {
        if (in_multi) {
            comment++;
            if (strstr(line, "*/")) in_multi = 0;
        } else if (strstr(line, "/*")) {
            comment++;
            in_multi = !(strstr(line, "*/"));
        } else if (is_blank(line)) {
            blank++;
        } else if (is_comment_start(line)) {
            comment++;
        } else {
            code++;
        }
    }
    fclose(f);
    printf("Blank: %d, Comment: %d, Code: %d\n", blank, comment, code);
}

// Q3: Count functions (skip if/for/while/switch)
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int has_control_keyword(const char *line) {
    const char *kw[] = {"if", "for", "while", "switch", "struct", "enum", "union"};
    char lower[500];
    for (int i = 0; line[i]; i++) lower[i] = tolower(line[i]);
    lower[strlen(line)] = 0;
    for (int i = 0; i < 7; i++) if (strstr(lower, kw[i])) return 1;
    return 0;
}

void q3(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return;
    char line[1000], prev[1000] = "";
    int count = 0, in_multi = 0;
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "/*")) in_multi = 1;
        if (in_multi) { if (strstr(line, "*/")) in_multi = 0; strcpy(prev, ""); continue; }
        if (line[0] == '/' && line[1] == '/') continue;
        if (strchr(line, '{') && (strchr(prev, ')') || strchr(line, ')')) && !has_control_keyword(line)) {
            count++;
        }
        strcpy(prev, line);
    }
    fclose(f);
    printf("Functions: %d\n", count);
}

// Q4: Count unique #include headers
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void q4(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return;
    char line[1000], headers[100][200];
    int hdr_count = 0, total = 0, in_multi = 0;
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "/*")) in_multi = 1;
        if (in_multi) { if (strstr(line, "*/")) in_multi = 0; continue; }
        if (line[0] == '/' && line[1] == '/') continue;
        int i = 0;
        while (isspace(line[i])) i++;
        if (line[i] != '#') continue;
        i++;
        while (isspace(line[i])) i++;
        if (strncmp(&line[i], "include", 7) != 0) continue;
        i += 7;
        while (isspace(line[i]) || line[i] == '<' || line[i] == '"') i++;
        char name[200], *j = name;
        while (line[i] && line[i] != '>' && line[i] != '"' && line[i] != '\n') *j++ = line[i++];
        *j = 0;
        if (j > name) {
            total++;
            int found = 0;
            for (int k = 0; k < hdr_count; k++) if (!strcmp(headers[k], name)) { found = 1; break; }
            if (!found && hdr_count < 100) strcpy(headers[hdr_count++], name);
        }
    }
    fclose(f);
    printf("Total includes: %d, Unique: %d\n", total, hdr_count);
    for (int i = 0; i < hdr_count; i++) printf("  %s\n", headers[i]);
}

// Q5: Count loops and conditionals (word-boundary aware)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char* remove_comments(const char *src) {
    char *out = malloc(strlen(src) + 1);
    int i = 0, j = 0, in_str = 0, in_char = 0, in_single = 0, in_multi = 0;
    while (src[i]) {
        if (!in_single && !in_multi && src[i] == '"' && (i == 0 || src[i-1] != '\\')) {
            in_str = !in_str; out[j++] = src[i++];
        } else if (in_str) {
            out[j++] = src[i++];
        } else if (!in_multi && src[i] == '/' && src[i+1] == '/') {
            in_single = 1; i += 2;
        } else if (in_single) {
            if (src[i] == '\n') { in_single = 0; out[j++] = '\n'; } i++;
        } else if (src[i] == '/' && src[i+1] == '*') {
            in_multi = 1; i += 2;
        } else if (in_multi && src[i] == '*' && src[i+1] == '/') {
            in_multi = 0; i += 2;
        } else if (!in_multi) {
            out[j++] = src[i++];
        } else {
            i++;
        }
    }
    out[j] = 0;
    return out;
}

int count_kw(const char *text, const char *kw) {
    int cnt = 0, klen = strlen(kw);
    const char *p = text;
    while ((p = strstr(p, kw))) {
        int ok = 1;
        if (p > text && (isalnum(p[-1]) || p[-1] == '_')) ok = 0;
        if (ok && (isalnum(p[klen]) || p[klen] == '_')) ok = 0;
        if (ok) cnt++;
        p++;
    }
    return cnt;
}

void q5(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = malloc(sz + 1);
    fread(buf, 1, sz, f);
    buf[sz] = 0;
    fclose(f);
    char *clean = remove_comments(buf);
    printf("for: %d, while: %d, do: %d, if: %d, switch: %d\n",
           count_kw(clean, "for"), count_kw(clean, "while"), count_kw(clean, "do"),
           count_kw(clean, "if"), count_kw(clean, "switch"));
    free(buf);
    free(clean);
}

// Q6: Count variables by type (basic)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int is_reserved(const char *w) {
    const char *kw[] = {"if","else","while","for","do","switch","return","sizeof","struct"};
    for (int i = 0; i < 9; i++) if (!strcmp(w, kw[i])) return 1;
    return 0;
}

void q6(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = malloc(sz + 1);
    fread(buf, 1, sz, f);
    buf[sz] = 0;
    fclose(f);
    
    int counts[9] = {0}; // int, float, double, char, long, short, unsigned, signed, void
    const char *types[] = {"int","float","double","char","long","short","unsigned","signed","void"};
    
    const char *p = buf;
    char word[256];
    while (*p) {
        while (isspace(*p)) p++;
        if (!*p) break;
        int i = 0;
        if (isalpha(*p) || *p == '_') {
            while (isalnum(*p) || *p == '_') word[i++] = *p++;
            word[i] = 0;
            int tidx = -1;
            for (int j = 0; j < 9; j++) if (!strcmp(word, types[j])) { tidx = j; break; }
            if (tidx >= 0) {
                while (isspace(*p) || *p == '*') p++;
                while (*p && *p != ';' && *p != ')') {
                    while (isspace(*p) || *p == '*') p++;
                    if (isalpha(*p) || *p == '_') {
                        int k = 0;
                        char name[256];
                        while (isalnum(*p) || *p == '_') name[k++] = *p++;
                        name[k] = 0;
                        while (isspace(*p)) p++;
                        if ((*p == '[' || *p == ',' || *p == ';' || *p == '=' || *p == ')') && !is_reserved(name)) {
                            counts[tidx]++;
                        }
                        if (*p == '(') { int pc = 1; p++; while (*p && pc) { pc += (*p == '(' ? 1 : (*p == ')' ? -1 : 0)); p++; } }
                    } else p++;
                    while (*p && *p != ',' && *p != ';' && *p != ')') p++;
                    if (*p == ',') p++; else break;
                }
            }
        } else p++;
    }
    printf("int: %d, float: %d, double: %d, char: %d, long: %d, short: %d, unsigned: %d, signed: %d, void: %d\n",
           counts[0], counts[1], counts[2], counts[3], counts[4], counts[5], counts[6], counts[7], counts[8]);
    free(buf);
}

// Q7: Remove extra spaces (preserve strings/comments)
#include <stdio.h>
#include <ctype.h>

void q7(const char *in, const char *out) {
    FILE *fi = fopen(in, "r"), *fo = fopen(out, "w");
    if (!fi || !fo) return;
    int ch, prev = 0, in_str = 0, in_char = 0, in_sl = 0, in_ml = 0, sp_wr = 0, at_start = 1;
    while ((ch = fgetc(fi)) != EOF) {
        if (ch == '"' && prev != '\\' && !in_char && !in_sl && !in_ml) { in_str = !in_str; fputc(ch, fo); sp_wr = 0; at_start = 0; }
        else if (ch == '\'' && prev != '\\' && !in_str && !in_sl && !in_ml) { in_char = !in_char; fputc(ch, fo); sp_wr = 0; at_start = 0; }
        else if (in_str || in_char) fputc(ch, fo);
        else if (ch == '/' && prev == '/' && !in_ml) { in_sl = 1; fputc(ch, fo); sp_wr = 0; }
        else if (in_sl) { fputc(ch, fo); if (ch == '\n') { in_sl = 0; at_start = 1; sp_wr = 0; } }
        else if (ch == '*' && prev == '/') { in_ml = 1; fputc(ch, fo); sp_wr = 0; }
        else if (in_ml) { fputc(ch, fo); if (ch == '/' && prev == '*') in_ml = 0; if (ch == '\n') { at_start = 1; sp_wr = 0; } }
        else if (ch == '\n') { fputc(ch, fo); at_start = 1; sp_wr = 0; }
        else if (isspace(ch)) { if (!at_start && !sp_wr) { fputc(' ', fo); sp_wr = 1; } }
        else { fputc(ch, fo); sp_wr = 0; at_start = 0; }
        prev = ch;
    }
    fclose(fi);
    fclose(fo);
}