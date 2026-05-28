**Concept**: Count lines, words and characters in a C source file
**Logic**:
- Read the full file into a string
- Count lines using newline characters
- Count words using whitespace split
- Count characters using string length
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
Input file: /home/devniru2704/Personal Files/Programs/Github/cc/code/sample_inputs/01_sample_input.c
Lines: 15
Words: 30
Characters: 194
```
**Run**:
```bash
python3 code/01_count_lines_words_chars.py
```
