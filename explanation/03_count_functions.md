**Concept**: Count function definitions in a C program snippet
**Logic**:
- Store a sample C program inside the Python file
- Use a regex that matches typical function headers ending with {
- Count all matches
**Sample Input**:
Embedded source used in the program:
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
Embedded Source:
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

Functions: 3
```
**Run**:
```bash
python3 code/03_count_functions.py
```
