#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1000

typedef struct {
    int int_count;
    int float_count;
    int double_count;
    int char_count;
    int long_count;
    int short_count;
    int unsigned_count;
    int signed_count;
    int void_count;
} TypeCount;

int is_keyword(char *word) {
    char *keywords[] = {"if", "else", "while", "for", "do", "switch", "case", 
                        "break", "continue", "return", "sizeof", "typedef",
                        "struct", "union", "enum", "const", "static", "extern",
                        "auto", "register", "volatile", "goto", "default"};
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void count_variables_by_type(char *content, TypeCount *counts) {
    char *types[] = {"int", "float", "double", "char", "long", "short", 
                     "unsigned", "signed", "void"};
    int num_types = 9;
    
    char *ptr = content;
    char word[100];
    
    while (*ptr != '\0') {
        // Skip whitespace
        while (isspace(*ptr)) ptr++;
        
        if (*ptr == '\0') break;
        
        // Read a word
        int i = 0;
        if (isalpha(*ptr) || *ptr == '_') {
            while (isalnum(*ptr) || *ptr == '_') {
                word[i++] = *ptr++;
            }
            word[i] = '\0';
            
            // Check if it's a data type
            int type_index = -1;
            for (int j = 0; j < num_types; j++) {
                if (strcmp(word, types[j]) == 0) {
                    type_index = j;
                    break;
                }
            }
            
            if (type_index != -1) {
                // Skip whitespace and pointers
                while (isspace(*ptr) || *ptr == '*') ptr++;
                
                // Read variable name(s)
                while (*ptr != ';' && *ptr != ')' && *ptr != '\0') {
                    // Skip whitespace
                    while (isspace(*ptr) || *ptr == '*') ptr++;
                    
                    if (*ptr == '\0' || *ptr == ';' || *ptr == ')') break;
                    
                    // Check for array brackets or function parenthesis
                    int is_variable = 0;
                    i = 0;
                    if (isalpha(*ptr) || *ptr == '_') {
                        while (isalnum(*ptr) || *ptr == '_') {
                            word[i++] = *ptr++;
                        }
                        word[i] = '\0';
                        
                        // Skip whitespace
                        while (isspace(*ptr)) ptr++;
                        
                        // Check if it's followed by (, [, =, or ,
                        if (*ptr == '(' && *(ptr-strlen(word)-1) != '(') {
                            // Might be a function, skip it
                            int paren_count = 1;
                            ptr++;
                            while (*ptr != '\0' && paren_count > 0) {
                                if (*ptr == '(') paren_count++;
                                if (*ptr == ')') paren_count--;
                                ptr++;
                            }
                        } else if (*ptr == '[' || *ptr == '=' || *ptr == ',' || *ptr == ';') {
                            is_variable = 1;
                        } else if (*ptr == ')') {
                            // Parameter in function
                            is_variable = 1;
                        }
                        
                        if (is_variable && !is_keyword(word)) {
                            switch(type_index) {
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
                    
                    // Skip to next comma or semicolon
                    while (*ptr != ',' && *ptr != ';' && *ptr != ')' && *ptr != '\0') {
                        if (*ptr == '[') {
                            while (*ptr != ']' && *ptr != '\0') ptr++;
                        }
                        if (*ptr == '=') {
                            // Skip initialization
                            ptr++;
                            int brace_count = 0;
                            while (*ptr != '\0') {
                                if (*ptr == '{') brace_count++;
                                if (*ptr == '}') brace_count--;
                                if (*ptr == ',' && brace_count == 0) break;
                                if (*ptr == ';' && brace_count == 0) break;
                                ptr++;
                            }
                            break;
                        }
                        ptr++;
                    }
                    
                    if (*ptr == ',') {
                        ptr++;
                    } else {
                        break;
                    }
                }
            }
        } else {
            ptr++;
        }
    }
}

int main() {
    FILE *file;
    char filename[100];
    long file_size;
    char *content;
    TypeCount counts = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    printf("Enter the C source filename to analyze: ");
    scanf("%s", filename);
    
    file = fopen(filename, "r");
    
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return 1;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Allocate memory and read file
    content = (char *)malloc(file_size + 1);
    if (content == NULL) {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return 1;
    }
    
    fread(content, 1, file_size, file);
    content[file_size] = '\0';
    
    fclose(file);
    
    // Remove comments
    char *cleaned = (char *)malloc(file_size + 1);
    int i = 0, j = 0;
    int in_single_comment = 0;
    int in_multi_comment = 0;
    int in_string = 0;
    
    while (i < file_size) {
        if (content[i] == '"' && (i == 0 || content[i-1] != '\\')) {
            in_string = !in_string;
            cleaned[j++] = ' ';
            i++;
            continue;
        }
        
        if (in_string) {
            cleaned[j++] = ' ';
            i++;
            continue;
        }
        
        if (!in_multi_comment && content[i] == '/' && content[i+1] == '/') {
            in_single_comment = 1;
            i += 2;
            continue;
        }
        
        if (in_single_comment) {
            if (content[i] == '\n') {
                in_single_comment = 0;
                cleaned[j++] = '\n';
            }
            i++;
            continue;
        }
        
        if (!in_single_comment && content[i] == '/' && content[i+1] == '*') {
            in_multi_comment = 1;
            i += 2;
            continue;
        }
        
        if (in_multi_comment) {
            if (content[i] == '*' && content[i+1] == '/') {
                in_multi_comment = 0;
                i += 2;
            } else {
                i++;
            }
            continue;
        }
        
        cleaned[j++] = content[i++];
    }
    cleaned[j] = '\0';
    
    count_variables_by_type(cleaned, &counts);
    
    free(content);
    free(cleaned);
    
    printf("\n--- Variable Count by Data Type ---\n");
    printf("File: %s\n", filename);
    printf("\nVariable counts:\n");
    printf("  int: %d\n", counts.int_count);
    printf("  float: %d\n", counts.float_count);
    printf("  double: %d\n", counts.double_count);
    printf("  char: %d\n", counts.char_count);
    printf("  long: %d\n", counts.long_count);
    printf("  short: %d\n", counts.short_count);
    printf("  unsigned: %d\n", counts.unsigned_count);
    printf("  signed: %d\n", counts.signed_count);
    printf("  void: %d\n", counts.void_count);
    printf("\nTotal variables: %d\n", 
           counts.int_count + counts.float_count + counts.double_count + 
           counts.char_count + counts.long_count + counts.short_count +
           counts.unsigned_count + counts.signed_count);
    
    return 0;
}
