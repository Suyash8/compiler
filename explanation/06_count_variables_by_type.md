**Concept**: Count variable declarations by basic data types in a C snippet
**Logic**:
- Store a sample C program inside the Python file
- Match each type keyword up to the semicolon
- Count matches per type
**Sample Input**:
Embedded source used in the program:
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
Embedded Source:
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
