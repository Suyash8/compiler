**Concept**: Count lines, words and characters in a C source snippet
**Logic**:
- Store a sample C program inside the Python file
- Count lines using newline characters
- Count words using whitespace split
- Count characters using string length
**Sample Input**:
Embedded source used in the program:
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
Embedded Source:
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

Lines: 15
Words: 30
Characters: 194
```
**Run**:
```bash
python3 code/01_count_lines_words_chars.py
```
