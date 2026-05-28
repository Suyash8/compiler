**Concept**: Count blank lines and comments in a C source snippet
**Logic**:
- Store a sample C program inside the Python file
- Count blank lines using newline characters
- Find // comments and /* */ comments using regex
**Sample Input**:
Embedded source used in the program:
```c
#include <stdio.h>

// this is a single line comment
int add(int a, int b) { return a + b; }

/*
	this is a
	multi-line comment
*/

int main()
{

	 // blank line above
	 return 0;
}
```
**Sample Output**:
```
Embedded Source:
#include <stdio.h>

// this is a single line comment
int add(int a, int b) { return a + b; }

/*
	this is a
	multi-line comment
*/

int main()
{

	 // blank line above
	 return 0;
}

Blank lines: 16
Single-line comments: 2
Multi-line comments: 1
```
**Run**:
```bash
python3 code/02_count_blanks_comments.py
```
