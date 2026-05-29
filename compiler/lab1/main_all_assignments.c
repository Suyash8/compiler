/*
  Short, beginner-friendly single-main program that performs simple
  analyses (Q1-Q7) on a C source file. Heuristics are intentionally
  simple to keep the code short and easy to read.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(void) {
    char fname[256];
    printf("File to analyze: ");
    if (scanf("%255s", fname) != 1) return 1;

    FILE *f = fopen(fname, "rb");
    if (!f) { perror("open"); return 1; }
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *s = malloc(n + 1);
    if (!s) { fclose(f); puts("OOM"); return 1; }
    fread(s, 1, n, f); s[n] = '\0'; fclose(f);

    /* make a cleaned copy: remove comments and string/char contents */
    char *c = malloc(n + 1);
    int in_sl = 0, in_ml = 0, in_sq = 0, in_dq = 0;
    for (long i = 0; i < n; ++i) {
        char a = s[i], b = (i+1<n)?s[i+1]:'\0';
        if (!in_sq && !in_dq && !in_ml && a=='/' && b=='/') { in_sl = 1; c[i] = ' '; continue; }
        if (!in_sq && !in_dq && !in_sl && a=='/' && b=='*') { in_ml = 1; c[i] = ' '; continue; }
        if (in_sl) { c[i] = (a=='\n') ? '\n' : ' '; if (a=='\n') in_sl = 0; continue; }
        if (in_ml) { c[i] = (a=='\n') ? '\n' : ' '; if (a=='*' && b=='/') { c[i+1]=' '; in_ml=0; i++; } continue; }
        if (!in_sq && a=='"' && (i==0 || s[i-1]!='\\')) { in_dq = !in_dq; c[i] = ' '; continue; }
        if (!in_dq && a=='\'' && (i==0 || s[i-1]!='\\')) { in_sq = !in_sq; c[i] = ' '; continue; }
        if (in_sq || in_dq) { c[i] = ' '; continue; }
        c[i] = a;
    }
    c[n] = '\0';

    /* Q1: lines, words, chars */
    int lines = 0, words = 0, chars = 0, inword = 0;
    for (long i = 0; i < n; ++i) {
        char ch = s[i]; chars++;
        if (ch == '\n') lines++;
        if (isspace((unsigned char)ch)) inword = 0; else if (!inword) { inword = 1; words++; }
    }

    /* Q2: blank / comment / code lines (simple) */
    int blank = 0, comment = 0, code = 0;
    long i = 0; in_ml = 0;
    while (i < n) {
        int hasCode = 0, hasComment = 0;
        while (i < n && s[i] != '\n') {
            if (!in_ml && s[i]=='/' && i+1<n && s[i+1]=='*') { in_ml = 1; hasComment = 1; i+=2; continue; }
            if (in_ml) { hasComment = 1; if (s[i]=='*' && i+1<n && s[i+1]=='/') { in_ml = 0; i+=2; continue; } i++; continue; }
            if (s[i]=='/' && i+1<n && s[i+1]=='/') { hasComment = 1; i+=2; while (i<n && s[i]!='\n') i++; break; }
            if (!isspace((unsigned char)s[i])) hasCode = 1;
            i++;
        }
        if (!hasCode && !hasComment) blank++; else if (hasComment && !hasCode) comment++; else code++;
        if (i<n && s[i]=='\n') i++;
    }

    /* Q4: headers (#include lines) */
    int includes = 0;
    char *p = c;
    while ((p = strstr(p, "#include")) ) { includes++; p += 8; }

    /* Q3/Q5: simple token counts using cleaned text */
    int funcs = 0, f_for = 0, f_while = 0, f_do = 0, f_if = 0, f_switch = 0;
    for (long k = 0; k < n; ++k) {
        if (c[k]==')') {
            long j = k+1; while (j<n && isspace((unsigned char)c[j])) j++;
            if (j<n && c[j]=='{') funcs++; /* naive: ) followed by { */
        }
        if (isalpha((unsigned char)c[k])) {
            char buf[16]; int t=0; long q=k;
            while (q<n && (isalnum((unsigned char)c[q])||c[q]=='_') && t<15) buf[t++]=c[q++];
            buf[t]=0;
            if (strcmp(buf,"for")==0) f_for++;
            else if (strcmp(buf,"while")==0) f_while++;
            else if (strcmp(buf,"do")==0) f_do++;
            else if (strcmp(buf,"if")==0) f_if++;
            else if (strcmp(buf,"switch")==0) f_switch++;
        }
    }

    /* Q6: count basic type keywords (very simple heuristic) */
    int t_int=0, t_float=0, t_double=0, t_char=0;
    p = c;
    while (*p) {
        if (strncmp(p,"int",3)==0 && !isalnum((unsigned char)p[3])) { t_int++; p+=3; continue; }
        if (strncmp(p,"float",5)==0 && !isalnum((unsigned char)p[5])) { t_float++; p+=5; continue; }
        if (strncmp(p,"double",6)==0 && !isalnum((unsigned char)p[6])) { t_double++; p+=6; continue; }
        if (strncmp(p,"char",4)==0 && !isalnum((unsigned char)p[4])) { t_char++; p+=4; continue; }
        p++;
    }

    /* Q7: write a simple collapsed-space file */
    char out[300]; snprintf(out, sizeof(out), "%s_nospaces.c", fname);
    FILE *o = fopen(out, "w");
    if (o) {
        int last_space = 0;
        for (long r = 0; r < n; ++r) {
            char ch = s[r];
            if (ch=='\t') ch=' ';
            if (ch==' ' || ch=='\n') { if (!last_space || ch=='\n') { fputc(ch, o); last_space = (ch==' '); } else continue; }
            else { fputc(ch, o); last_space = 0; }
        }
        fclose(o);
    }

    /* Print results */
    printf("\nQ1: lines=%d words=%d chars=%d\n", lines, words, chars);
    printf("Q2: blank=%d comment=%d code=%d\n", blank, comment, code);
    printf("Q3: functions (naive)= %d\n", funcs);
    printf("Q4: #includes= %d\n", includes);
    printf("Q5: for=%d while=%d do=%d if=%d switch=%d\n", f_for, f_while, f_do, f_if, f_switch);
    printf("Q6: int=%d float=%d double=%d char=%d\n", t_int, t_float, t_double, t_char);
    printf("Q7: output written to %s\n", out);

    free(s); free(c);
    return 0;
}
