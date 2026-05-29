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
