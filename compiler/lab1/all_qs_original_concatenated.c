/* ============================================================================
   Q1: Write a C program to count lines, words, and characters
   ============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main() {
    FILE *file;
    char* filename = "sample_test.c";
    char ch;
    int lines = 0, words = 0, characters = 0;
    int in_word = 0;
    
    file = fopen(filename, "r");
    
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return 1;
    }
    
    while ((ch = fgetc(file)) != EOF) {
        characters++;
        
        if (ch == '\n') {
            lines++;
        }
        
        if (isspace(ch)) {
            in_word = 0;
        } else if (in_word == 0) {
            in_word = 1;
            words++;
        }
    }
    
    fclose(file);
    
    printf("File: %s\nLines: %d\nWords: %d\nCharacters: %d\n", filename, lines, words, characters);
    
    return 0;
}

/* ============================================================================
   Q2: Write a C program to count blank lines, comment lines, and code lines
   ============================================================================ */
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

/* ============================================================================
   Q3: Write a C program to count the number of functions
   ============================================================================ */
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

/* ============================================================================
   Q4: Write a C program to count the number of header files
   ============================================================================ */
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

/* ============================================================================
   Q5: Write a C program to count loops and conditional statements
   ============================================================================ */
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

/* ============================================================================
   Q6: Write a C program to count variables by data types
   ============================================================================ */
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

/* ============================================================================
   Q7: Write a C program to remove extra spaces from source code
   ============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1000

void remove_extra_spaces(FILE *input, FILE *output) {
    char ch, prev_ch = '\0';
    int in_string = 0;
    int in_char = 0;
    int in_single_comment = 0;
    int in_multi_comment = 0;
    int space_written = 0;
    int at_line_start = 1;
    
    while ((ch = fgetc(input)) != EOF) {
        // Handle string literals
        if (ch == '"' && prev_ch != '\\' && !in_char && !in_single_comment && !in_multi_comment) {
            in_string = !in_string;
            fputc(ch, output);
            prev_ch = ch;
            space_written = 0;
            at_line_start = 0;
            continue;
        }
        
        // Handle character literals
        if (ch == '\'' && prev_ch != '\\' && !in_string && !in_single_comment && !in_multi_comment) {
            in_char = !in_char;
            fputc(ch, output);
            prev_ch = ch;
            space_written = 0;
            at_line_start = 0;
            continue;
        }
        
        // Inside string or char literal, preserve everything
        if (in_string || in_char) {
            fputc(ch, output);
            prev_ch = ch;
            continue;
        }
        
        // Handle single-line comments
        if (ch == '/' && prev_ch == '/' && !in_multi_comment) {
            in_single_comment = 1;
            fputc(ch, output);
            prev_ch = ch;
            space_written = 0;
            continue;
        }
        
        if (in_single_comment) {
            fputc(ch, output);
            if (ch == '\n') {
                in_single_comment = 0;
                at_line_start = 1;
                space_written = 0;
            }
            prev_ch = ch;
            continue;
        }
        
        // Handle multi-line comments
        if (ch == '*' && prev_ch == '/' && !in_single_comment) {
            in_multi_comment = 1;
            fputc(ch, output);
            prev_ch = ch;
            space_written = 0;
            continue;
        }
        
        if (in_multi_comment) {
            fputc(ch, output);
            if (ch == '/' && prev_ch == '*') {
                in_multi_comment = 0;
            }
            if (ch == '\n') {
                at_line_start = 1;
                space_written = 0;
            } else if (!isspace(ch)) {
                at_line_start = 0;
            }
            prev_ch = ch;
            continue;
        }
        
        // Handle newlines
        if (ch == '\n') {
            fputc(ch, output);
            at_line_start = 1;
            space_written = 0;
            prev_ch = ch;
            continue;
        }
        
        // Handle spaces and tabs
        if (isspace(ch)) {
            // Skip leading spaces on a line
            if (at_line_start) {
                prev_ch = ch;
                continue;
            }
            
            // Write only one space for consecutive spaces
            if (!space_written) {
                fputc(' ', output);
                space_written = 1;
            }
            prev_ch = ch;
            continue;
        }
        
        // Regular character
        fputc(ch, output);
        space_written = 0;
        at_line_start = 0;
        prev_ch = ch;
    }
}

int main() {
    FILE *input_file, *output_file;
    char input_filename[100];
    char output_filename[100];
    
    printf("Enter the input C source filename: ");
    scanf("%s", input_filename);
    
    printf("Enter the output filename: ");
    scanf("%s", output_filename);
    
    input_file = fopen(input_filename, "r");
    
    if (input_file == NULL) {
        printf("Error: Could not open file %s\n", input_filename);
        return 1;
    }
    
    output_file = fopen(output_filename, "w");
    
    if (output_file == NULL) {
        printf("Error: Could not create file %s\n", output_filename);
        fclose(input_file);
        return 1;
    }
    
    remove_extra_spaces(input_file, output_file);
    
    fclose(input_file);
    fclose(output_file);
    
    printf("\n--- Space Removal Complete ---\n");
    printf("Input file: %s\n", input_filename);
    printf("Output file: %s\n", output_filename);
    printf("Extra spaces have been removed successfully!\n");
    
    return 0;
}
