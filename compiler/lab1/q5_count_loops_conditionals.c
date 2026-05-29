#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1000

int count_keyword(char *content, char *keyword) {
    int count = 0;
    char *ptr = content;
    int keyword_len = strlen(keyword);
    
    while ((ptr = strstr(ptr, keyword)) != NULL) {
        // Check if it's a whole word (not part of another identifier)
        int is_whole_word = 1;
        
        // Check character before
        if (ptr != content) {
            char prev = *(ptr - 1);
            if (isalnum(prev) || prev == '_') {
                is_whole_word = 0;
            }
        }
        
        // Check character after
        if (is_whole_word) {
            char next = *(ptr + keyword_len);
            if (isalnum(next) || next == '_') {
                is_whole_word = 0;
            }
        }
        
        if (is_whole_word) {
            count++;
        }
        
        ptr++;
    }
    
    return count;
}

int main() {
    FILE *file;
    char filename[100];
    char ch;
    char *content;
    long file_size;
    
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
    
    // Remove comments to avoid counting keywords in comments
    char *cleaned = (char *)malloc(file_size + 1);
    int i = 0, j = 0;
    int in_single_comment = 0;
    int in_multi_comment = 0;
    int in_string = 0;
    
    while (i < file_size) {
        // Handle string literals
        if (content[i] == '"' && (i == 0 || content[i-1] != '\\')) {
            in_string = !in_string;
            cleaned[j++] = content[i++];
            continue;
        }
        
        if (in_string) {
            cleaned[j++] = content[i++];
            continue;
        }
        
        // Handle single-line comments
        if (!in_multi_comment && content[i] == '/' && content[i+1] == '/') {
            in_single_comment = 1;
            i += 2;
            continue;
        }
        
        if (in_single_comment) {
            if (content[i] == '\n') {
                in_single_comment = 0;
                cleaned[j++] = content[i];
            }
            i++;
            continue;
        }
        
        // Handle multi-line comments
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
    
    // Count loops
    int for_loops = count_keyword(cleaned, "for");
    int while_loops = count_keyword(cleaned, "while");
    int do_loops = count_keyword(cleaned, "do");
    
    // Count conditionals
    int if_statements = count_keyword(cleaned, "if");
    int switch_statements = count_keyword(cleaned, "switch");
    
    free(content);
    free(cleaned);
    
    printf("\n--- Loop and Conditional Count ---\n");
    printf("File: %s\n", filename);
    printf("\nLoops:\n");
    printf("  for loops: %d\n", for_loops);
    printf("  while loops: %d\n", while_loops);
    printf("  do-while loops: %d\n", do_loops);
    printf("  Total loops: %d\n", for_loops + while_loops + do_loops);
    printf("\nConditional Statements:\n");
    printf("  if statements: %d\n", if_statements);
    printf("  switch statements: %d\n", switch_statements);
    printf("  Total conditionals: %d\n", if_statements + switch_statements);
    
    return 0;
}
