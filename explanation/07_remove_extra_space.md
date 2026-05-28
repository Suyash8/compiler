**Concept**: Remove extra spaces and blank lines from source code
**Logic**:
- Trim leading and trailing spaces
- Collapse multiple spaces into one
- Remove consecutive blank lines
**Sample Input**:
File: code/sample_inputs/07_sample_input.c
```c
#include <stdio.h>

int main()
{

	printf("Hello");

	return 0;
}
```
**Sample Output**:
```
Input file: /home/devniru2704/Personal Files/Programs/Github/cc/code/sample_inputs/07_sample_input.c
Cleaned output:
#include <stdio.h>

int main()
{

printf("Hello");

return 0;
}
```
**Run**:
```bash
python3 code/07_remove_extra_space.py
```
