#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_LEXEME 256

const char *keywords[] = {
"int","float","char","double","if","else","while","for",
"return","void","break","continue","long","short","signed","unsigned"
};

const int KW_COUNT = (int)(sizeof(keywords)/sizeof(keywords[0]));

int is_keyword(const char *s) {
    for (int i = 0; i < KW_COUNT; i++) {
        if (strcmp(s, keywords[i]) == 0) return 1;
    }
    return 0;
}

int is_delimiter(int c) {
    return (c=='(' || c==')' || c=='{' || c=='}' || c=='[' || c==']' ||
            c==';' || c==',' );
}

int is_single_op(int c) {
    return (c=='+' || c=='-' || c=='*' || c=='/' || c=='%' ||
            c=='=' || c=='<' || c=='>' || c=='!' );
}

int print_token(const char *type, const char *lexeme, int line) {
    printf("[Line %d] %-12s : %s\n", line, type, lexeme);
    return 0;
}

int getc_track(FILE *fp, int *line) {
    int c = fgetc(fp);
    if (c == '\n') (*line)++;
    return c;
}

void ungetc_track(int c, FILE *fp, int *line) {
    if (c == EOF) return;
    if (c == '\n') (*line)--;
    ungetc(c, fp);
}

int main(int argc, char *argv[]) {

    FILE *fp = stdin;
    int line = 1;

    if (argc == 2) {
        fp = fopen(argv[1], "r");
        if (!fp) {
            perror("Error opening file");
            return 1;
        }
    }

    int c;

    while ((c = getc_track(fp, &line)) != EOF) {

        if (isspace(c)) continue;

        if (isalpha(c) || c == '_') {

            char lex[MAX_LEXEME];
            int k = 0;
            lex[k++] = (char)c;

            while ((c = getc_track(fp, &line)) != EOF &&
                   (isalnum(c) || c == '_')) {
                if (k < MAX_LEXEME - 1) lex[k++] = (char)c;
            }

            lex[k] = '\0';
            ungetc_track(c, fp, &line);

            if (is_keyword(lex))
                print_token("KEYWORD", lex, line);
            else
                print_token("IDENTIFIER", lex, line);

            continue;
        }

        if (isdigit(c)) {

            char lex[MAX_LEXEME];
            int k = 0;
            int isFloat = 0;
            lex[k++] = (char)c;

            while ((c = getc_track(fp, &line)) != EOF) {

                if (isdigit(c)) {
                    if (k < MAX_LEXEME - 1) lex[k++] = (char)c;
                }

                else if (c == '.' && !isFloat) {
                    isFloat = 1;
                    if (k < MAX_LEXEME - 1) lex[k++] = (char)c;
                }

                else {
                    break;
                }
            }

            lex[k] = '\0';
            ungetc_track(c, fp, &line);

            if (isFloat)
                print_token("FLOAT", lex, line);
            else
                print_token("INTEGER", lex, line);

            continue;
        }

        if (c == '"') {

            char lex[MAX_LEXEME];
            int k = 0;
            lex[k++] = '"';
            int ok = 0;

            while ((c = getc_track(fp, &line)) != EOF) {

                if (k < MAX_LEXEME - 1) lex[k++] = (char)c;

                if (c == '\\') {
                    int nxt = getc_track(fp, &line);
                    if (nxt == EOF) break;
                    if (k < MAX_LEXEME - 1) lex[k++] = (char)nxt;
                    continue;
                }

                if (c == '"') {
                    ok = 1;
                    break;
                }

                if (c == '\n') break;
            }

            lex[k] = '\0';

            if (ok)
                print_token("STRING", lex, line);
            else
                print_token("ERROR", "Unterminated string literal", line);

            continue;
        }

        if (c == '\'') {

            char lex[MAX_LEXEME];
            int k = 0;
            lex[k++] = '\'';
            int ok = 0;

            while ((c = getc_track(fp, &line)) != EOF) {

                if (k < MAX_LEXEME - 1) lex[k++] = (char)c;

                if (c == '\\') {
                    int nxt = getc_track(fp, &line);
                    if (nxt == EOF) break;
                    if (k < MAX_LEXEME - 1) lex[k++] = (char)nxt;
                    continue;
                }

                if (c == '\'') {
                    ok = 1;
                    break;
                }

                if (c == '\n') break;
            }

            lex[k] = '\0';

            if (ok)
                print_token("CHAR", lex, line);
            else
                print_token("ERROR", "Unterminated char literal", line);

            continue;
        }

        if (c == '/') {

            int nxt = getc_track(fp, &line);

            if (nxt == '/') {

                while ((c = getc_track(fp, &line)) != EOF && c != '\n') {}

                print_token("COMMENT", "//...", line);
                continue;
            }

            else if (nxt == '*') {

                int prev = 0;
                int closed = 0;

                while ((c = getc_track(fp, &line)) != EOF) {
                    if (prev == '*' && c == '/') {
                        closed = 1;
                        break;
                    }
                    prev = c;
                }

                if (closed)
                    print_token("COMMENT", "/*...*/", line);
                else
                    print_token("ERROR", "Unterminated block comment", line);

                continue;
            }

            else {
                ungetc_track(nxt, fp, &line);
                print_token("OPERATOR", "/", line);
                continue;
            }
        }

        if (is_single_op(c)) {

            int nxt = getc_track(fp, &line);

            char op2[3] = {(char)c, (char)nxt, '\0'};
            int isTwoChar = 0;

            if (!strcmp(op2, "==") ||
                !strcmp(op2, "!=") ||
                !strcmp(op2, "<=") ||
                !strcmp(op2, ">=") ||
                !strcmp(op2, "++") ||
                !strcmp(op2, "--") ||
                !strcmp(op2, "&&") ||
                !strcmp(op2, "||")) {

                isTwoChar = 1;
                print_token("OPERATOR", op2, line);
            }

            if (!isTwoChar) {

                ungetc_track(nxt, fp, &line);

                char op1[2] = {(char)c, '\0'};
                print_token("OPERATOR", op1, line);
            }

            continue;
        }

        if (is_delimiter(c)) {

            char d[2] = {(char)c, '\0'};
            print_token("DELIMITER", d, line);
            continue;
        }

        if (c == '&' || c == '|') {

            int nxt = getc_track(fp, &line);

            char op2[3] = {(char)c, (char)nxt, '\0'};

            if (!strcmp(op2, "&&") || !strcmp(op2, "||")) {
                print_token("OPERATOR", op2, line);
            }

            else {
                ungetc_track(nxt, fp, &line);
                char op1[2] = {(char)c, '\0'};
                print_token("UNKNOWN", op1, line);
            }

            continue;
        }

        {
            char unk[2] = {(char)c, '\0'};
            print_token("UNKNOWN", unk, line);
        }
    }

    if (fp != stdin) fclose(fp);

    return 0;
}