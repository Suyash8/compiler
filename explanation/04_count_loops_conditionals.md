**Concept**: Count loop and conditional statements in a C snippet
**Logic**:
- Store a sample C program inside the Python file
- Use regex to find loop keywords and conditional keywords
- Count occurrences
**Sample Input**:
Embedded source used in the program:
```c
#include <stdio.h>

int main()
{
	int i;
	for (i = 0; i < 3; i++)
	{
	}
	i = 0;
	while (i < 2)
	{
		i++;
	}
	int j = 0;
	do
	{
		j++;
	} while (j < 1);

	if (i > 0)
	{
	}
	else if (i == 0)
	{
	}
	else
	{
	}

	switch (i)
	{
	case 0:
		break;
	default:
		break;
	}

	return 0;
}
```
**Sample Output**:
```
Embedded Source:
#include <stdio.h>

int main()
{
	int i;
	for (i = 0; i < 3; i++)
	{
	}
	i = 0;
	while (i < 2)
	{
		i++;
	}
	int j = 0;
	do
	{
		j++;
	} while (j < 1);

	if (i > 0)
	{
	}
	else if (i == 0)
	{
	}
	else
	{
	}

	switch (i)
	{
	case 0:
		break;
	default:
		break;
	}

	return 0;
}

Loops: 4
Conditionals: 3
```
**Run**:
```bash
python3 code/04_count_loops_conditionals.py
```
