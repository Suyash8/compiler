**Concept**: Regex-based lexical analyzer in C
**Logic**:
- Read the full source file into memory
- Check the current text against regex rules for keywords, identifiers, numbers, strings, operators, delimiters, and comments
- Print the token type and token text in sequence
**Sample Input**:
File: code/sample_inputs/01_sample_input.c
```c
#include <stdio.h>

int add(int a, int b)
{
	return a + b;
}

int main()
{
	printf("Line one\n");
	printf("Line two\n");
	int x = add(2, 3);
	printf("x=%d\n", x);
	return 0;
}
```
**Sample Output**:
```
Input file: code/sample_inputs/01_sample_input.c
Tokens:
PREPROCESSOR #include <stdio.h>
KEYWORD      int
IDENTIFIER   add
DELIMITER    (
KEYWORD      int
IDENTIFIER   a
DELIMITER    ,
KEYWORD      int
IDENTIFIER   b
DELIMITER    )
DELIMITER    {
KEYWORD      return
IDENTIFIER   a
OPERATOR     +
IDENTIFIER   b
DELIMITER    ;
DELIMITER    }
KEYWORD      int
KEYWORD      main
DELIMITER    (
DELIMITER    )
DELIMITER    {
IDENTIFIER   printf
DELIMITER    (
STRING       "Line one\n"
DELIMITER    )
DELIMITER    ;
IDENTIFIER   printf
DELIMITER    (
STRING       "Line two\n"
DELIMITER    )
DELIMITER    ;
KEYWORD      int
IDENTIFIER   x
OPERATOR     =
IDENTIFIER   add
DELIMITER    (
NUMBER       2
DELIMITER    ,
NUMBER       3
DELIMITER    )
DELIMITER    ;
IDENTIFIER   printf
DELIMITER    (
STRING       "x=%d\n"
DELIMITER    ,
IDENTIFIER   x
DELIMITER    )
DELIMITER    ;
KEYWORD      return
NUMBER       0
DELIMITER    ;
DELIMITER    }
```
**Run**:
```bash
gcc -std=c99 -Wall code/12_regex_lexical_c.c -o 12_regex_lexical_c
./12_regex_lexical_c code/sample_inputs/01_sample_input.c
```
