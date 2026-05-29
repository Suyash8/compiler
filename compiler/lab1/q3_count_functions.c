#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1000

int count_functions(FILE *file) {
    char line[MAX_LINE];
    char prev_line[MAX_LINE] = "";
    int function_count = 0;
    int in_multiline_comment = 0;
    
    while (fgets(line, MAX_LINE, file) != NULL) {
        // Handle multi-line comments
        if (strstr(line, "/*") != NULL) {
            in_multiline_comment = 1;
        }
        if (in_multiline_comment) {
            if (strstr(line, "*/") != NULL) {
                in_multiline_comment = 0;
            }
            continue;
        }
        
        // Skip single-line comments and blank lines
        int i = 0;
        while (isspace(line[i])) i++;
        
        if (line[i] == '/' && line[i+1] == '/') {
            continue;
        }
        if (line[i] == '\0' || line[i] == '\n') {
            continue;
        }
        
        // Look for function definitions
        // A function typically has: return_type function_name(parameters) {
        // We look for an opening brace at the start of a line or after some whitespace
        // that follows a line with a closing parenthesis
        
        if (strchr(line, '{') != NULL) {
            // Check if previous line or current line has a closing parenthesis
            if (strchr(prev_line, ')') != NULL || strchr(line, ')') != NULL) {
                // Additional check: make sure it's not a struct, enum, or control statement
                char temp[MAX_LINE];
                strcpy(temp, prev_line);
                strcat(temp, line);
                
                // Convert to lowercase for checking
                char lower[MAX_LINE * 2];
                int k = 0;
                for (int j = 0; temp[j] != '\0'; j++) {
                    lower[k++] = tolower(temp[j]);
                }
                lower[k] = '\0';
                
                // Skip if it's a control structure
                if (strstr(lower, "if") == NULL &&
                    strstr(lower, "for") == NULL &&
                    strstr(lower, "while") == NULL &&
                    strstr(lower, "switch") == NULL &&
                    strstr(lower, "struct") == NULL &&
                    strstr(lower, "enum") == NULL &&
                    strstr(lower, "union") == NULL) {
                    
                    // Likely a function definition
                    function_count++;
                }
            }
        }
        
        strcpy(prev_line, line);
    }
    
    return function_count;
}

int main() {
    FILE *file;
    char* filename = "sample_test.c";
    
    file = fopen(filename, "r");
    
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return 1;
    }
    
    int functions = count_functions(file);
    
    fclose(file);
    
    printf("File: %s\n", filename);
    printf("Number of Functions: %d\n", functions);
    
    return 0;
}
