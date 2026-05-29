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
