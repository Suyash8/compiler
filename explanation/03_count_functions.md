**Concept**: Count function definitions in a C program
**Logic**:
- Use a regex that matches typical function headers ending with {
- Count all matches
**Sample Input**:
File: code/sample_inputs/03_sample_input.c
```c
#include <stdio.h>

void foo()
{
	int x = 0;
}

int bar(int a)
{
	return a * 2;
}

int main()
{
	foo();
	int r = bar(5);
	printf("%d\n", r);
	return 0;
}
```
**Sample Output**:
```
Input file: /home/devniru2704/Personal Files/Programs/Github/cc/code/sample_inputs/03_sample_input.c
Functions: 3
```
**Run**:
```bash
python3 code/03_count_functions.py
```
