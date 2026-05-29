#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1000

int is_blank_line(char *line) {
    for (int i = 0; line[i] != '\0'; i++) {
        if (!isspace(line[i])) {
            return 0;
        }
    }
    return 1;
}

int is_comment_line(char *line) {
    int i = 0;
    
    // Skip leading whitespace
    while (isspace(line[i])) {
        i++;
    }
    
    // Check for single-line comment
    if (line[i] == '/' && line[i+1] == '/') {
        return 1;
    }
    
    // Check for multi-line comment start
    if (line[i] == '/' && line[i+1] == '*') {
        return 1;
    }
    
    return 0;
}

int main() {
    FILE *file;
    char* filename = "sample_test.c";
    char line[MAX_LINE];
    int blank_lines = 0, comment_lines = 0, code_lines = 0;
    int in_multiline_comment = 0;
    
    file = fopen(filename, "r");
    
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return 1;
    }
    
    while (fgets(line, MAX_LINE, file) != NULL) {
        // Check for multi-line comment end
        if (in_multiline_comment) {
            comment_lines++;
            if (strstr(line, "*/") != NULL) {
                in_multiline_comment = 0;
            }
            continue;
        }
        
        // Check for multi-line comment start
        if (strstr(line, "/*") != NULL) {
            comment_lines++;
            in_multiline_comment = 1;
            if (strstr(line, "*/") != NULL) {
                in_multiline_comment = 0;
            }
            continue;
        }
        
        // Check if blank line
        if (is_blank_line(line)) {
            blank_lines++;
        }
        // Check if comment line
        else if (is_comment_line(line)) {
            comment_lines++;
        }
        // Otherwise it's a code line
        else {
            code_lines++;
        }
    }
    
    fclose(file);
    
    printf("File: %s\n", filename);
    printf("Blank Lines: %d\n", blank_lines);
    printf("Comment Lines: %d\n", comment_lines);
    printf("Code Lines: %d\n", code_lines);
    printf("Total Lines: %d\n", blank_lines + comment_lines + code_lines);
    
    return 0;
}
