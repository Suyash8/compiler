**Concept**: Count variable declarations by basic data types
**Logic**:
- Match each type keyword up to the semicolon
- Count matches per type
**Sample Input**:
File: code/sample_inputs/06_sample_input.c
```c
#include <stdio.h>

int a;
char c;
float f;
double d;
long l;
short s;

int main()
{
	int x;
	char y;
	return 0;
}
```
**Sample Output**:
```
Input file: /home/devniru2704/Personal Files/Programs/Github/cc/code/sample_inputs/06_sample_input.c
int: 2
char: 2
float: 1
double: 1
long: 1
short: 1
```
**Run**:
```bash
python3 code/06_count_variables_by_type.py
```
