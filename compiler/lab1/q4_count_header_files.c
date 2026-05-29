#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1000

typedef struct {
    char name[100];
    int count;
} HeaderInfo;

int main() {
    FILE *file;
    char* filename = "sample_test.c";
    char line[MAX_LINE];
    HeaderInfo headers[100];
    int header_count = 0;
    int total_includes = 0;
    int in_multiline_comment = 0;
    
    file = fopen(filename, "r");
    
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return 1;
    }
    
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
        
        // Skip leading whitespace
        int i = 0;
        while (isspace(line[i])) i++;
        
        // Skip single-line comments
        if (line[i] == '/' && line[i+1] == '/') {
            continue;
        }
        
        // Check for #include directive
        if (line[i] == '#') {
            i++;
            while (isspace(line[i])) i++;
            
            // Check if it's an include statement
            if (strncmp(&line[i], "include", 7) == 0) {
                i += 7;
                while (isspace(line[i])) i++;
                
                // Extract header file name
                char header_name[100];
                int j = 0;
                
                // Skip opening bracket/quote
                if (line[i] == '<' || line[i] == '"') {
                    char closing = (line[i] == '<') ? '>' : '"';
                    i++;
                    
                    // Extract the header name
                    while (line[i] != closing && line[i] != '\0' && line[i] != '\n') {
                        header_name[j++] = line[i++];
                    }
                    header_name[j] = '\0';
                    
                    if (strlen(header_name) > 0) {
                        total_includes++;
                        
                        // Check if this header already exists in our list
                        int found = 0;
                        for (int k = 0; k < header_count; k++) {
                            if (strcmp(headers[k].name, header_name) == 0) {
                                headers[k].count++;
                                found = 1;
                                break;
                            }
                        }
                        
                        // If not found, add it to the list
                        if (!found) {
                            strcpy(headers[header_count].name, header_name);
                            headers[header_count].count = 1;
                            header_count++;
                        }
                    }
                }
            }
        }
    }
    
    fclose(file);
    
    printf("File: %s\n", filename);
    printf("\nTotal #include statements: %d\n", total_includes);
    printf("Unique header files: %d\n\n", header_count);
    
    if (header_count > 0) {
        printf("Header files included:\n");
        for (int i = 0; i < header_count; i++) {
            printf("  %s", headers[i].name);
            if (headers[i].count > 1) {
                printf(" (included %d times)", headers[i].count);
            }
            printf("\n");
        }
    } else {
        printf("No header files found.\n");
    }
    
    return 0;
}
