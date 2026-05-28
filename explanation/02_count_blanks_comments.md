**Concept**: Count blank lines and comments in a C source file
**Logic**:
- Read the full file into a string
- Count blank lines using newline characters
- Find // comments and /* */ comments using regex
**Sample Input**:
File: code/sample_inputs/02_sample_input.c
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
Input file: /home/devniru2704/Personal Files/Programs/Github/cc/code/sample_inputs/02_sample_input.c
Blank lines: 16
Single-line comments: 2
Multi-line comments: 1
```
**Run**:
```bash
python3 code/02_count_blanks_comments.py
```
