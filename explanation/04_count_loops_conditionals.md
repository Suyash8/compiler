**Concept**: Count loop and conditional statements
**Logic**:
- Use regex to find loop keywords and conditional keywords
- Count occurrences
**Sample Input**:
File: code/sample_inputs/04_sample_input.c
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
Input file: /home/devniru2704/Personal Files/Programs/Github/cc/code/sample_inputs/04_sample_input.c
Loops: 4
Conditionals: 3
```
**Run**:
```bash
python3 code/04_count_loops_conditionals.py
```
